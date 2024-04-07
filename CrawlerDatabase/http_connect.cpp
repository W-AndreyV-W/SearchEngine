#include "http_connect.h"

HTTPConnect::HTTPConnect(tcp::socket&& socket, ssl::context& ctx, std::shared_ptr<std::string const> const& doc_root, ReturnResponse* response, 
                         std::string htmlWebsite) : streamServer(std::move(socket), ctx), rootDirectory(doc_root), codWebsite(htmlWebsite) {
    
    returnResponse = response;
}

HTTPConnect::~HTTPConnect() {

 
}

void HTTPConnect::run() {

    net::dispatch(streamServer.get_executor(), beast::bind_front_handler(&HTTPConnect::sslConfirmation, shared_from_this()));
}

void HTTPConnect::sslConfirmation() {

    beast::get_lowest_layer(streamServer).expires_after(std::chrono::seconds(30));
    streamServer.async_handshake(ssl::stream_base::server, beast::bind_front_handler(&HTTPConnect::handshake, shared_from_this()));
}

void HTTPConnect::handshake(beast::error_code error) {

    if (error) {

        httpError("handshake", error);
        return;
    }

    emptyRequest();
}

void HTTPConnect::emptyRequest() {

    requestClient = {};
    beast::get_lowest_layer(streamServer).expires_after(std::chrono::seconds(30));
    http::async_read(streamServer, bufferServer, requestClient, beast::bind_front_handler(&HTTPConnect::readRequest, shared_from_this()));
}

void HTTPConnect::readRequest(beast::error_code error, std::size_t bytesTransferred) {
    
    boost::ignore_unused(bytesTransferred);

    if (error == http::error::end_of_stream) {

        closeConnect();
        return;
    }

    if (error) {

        httpError("read", error);
        return;
    }

    sendResponse(handleRequest(*rootDirectory, std::move(requestClient)));
}

void HTTPConnect::sendResponse(http::message_generator&& message) {

    bool keepAlive = message.keep_alive();
    beast::async_write(streamServer, std::move(message), beast::bind_front_handler(&HTTPConnect::writeRequest, this->shared_from_this(), keepAlive));
}

void HTTPConnect::writeRequest(bool keepAlive, beast::error_code error, std::size_t bytesTransferred) {

    boost::ignore_unused(bytesTransferred);

    if (error) {

        httpError("write", error);
        return;
    }

    if (!keepAlive) {

        return closeConnect();
    }

    emptyRequest();
}

void HTTPConnect::closeConnect() {

    beast::get_lowest_layer(streamServer).expires_after(std::chrono::seconds(30));
    streamServer.async_shutdown(beast::bind_front_handler(&HTTPConnect::shutdownConnect, shared_from_this()));
}

void HTTPConnect::shutdownConnect(beast::error_code error) {

    if (error) {

        httpError("shutdown", error);
        return;
    }
}

void HTTPConnect::httpError(char const* what, beast::error_code& errorMessage) {

    errorConnect = what + ' ' + errorMessage.message();
}


template <class Body, class Allocator>
http::message_generator HTTPConnect::handleRequest(beast::string_view rootDirectory, http::request<Body, 
                                                   http::basic_fields<Allocator>>&& request) {
    std::cout << request.body() << std::endl;
    if (request.method() != http::verb::get && request.method() != http::verb::post) {

        return messagesRequest(request, http::status::bad_request, "Unknown HTTP-method");
    }

    if (request.target().empty() || request.target()[0] != '/' || request.target().find("..") != beast::string_view::npos) {

        return messagesRequest(request, http::status::bad_request, "Illegal request-target");
    }

    if (request.body().substr(0, 5) == "text=") {

        std::string searchField = request.body().substr(5, request.body().size() - 5);
        return messagesRequest(request, http::status::ok, codWebsite + returnResponse->responseDatabese(searchField));
    }

    return messagesRequest(request, http::status::ok, codWebsite);
}

template <class Body, class Allocator>
http::message_generator HTTPConnect::messagesRequest(http::request<Body, http::basic_fields<Allocator>>& requestClient, 
                                                     http::status statusCode, beast::string_view messagesServer) {

    http::response<http::string_body> responseServer{ statusCode, requestClient.version() };
    responseServer.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    responseServer.set(http::field::content_type, "text/html");
    responseServer.keep_alive(requestClient.keep_alive());
    responseServer.body() = std::string(messagesServer);
    responseServer.prepare_payload();

    return responseServer;
}

std::string HTTPConnect::errorHTTPConnect() {

    return errorConnect;
}