#include "http_connection.h"


HTTPConnection::HTTPConnection(net::io_context& ioc, ssl::context& ctx, tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root, 
                               std::vector<std::string> connectionData, std::string htmlWebsite) : 
                               ioContext(ioc), sslContext(ctx), acceptorConnection(ioc), rootDirectory(doc_root) {

    codWebsite = htmlWebsite;
    returnResponse = new ReturnResponse(connectionData);

    beast::error_code error;
    acceptorConnection.open(endpoint.protocol(), error);

    if (error) {

        httpError("open", error);

        return;
    }

    acceptorConnection.set_option(net::socket_base::reuse_address(true), error);

    if (error) {

        httpError("set option", error);

        return;
    }

    acceptorConnection.bind(endpoint, error);

    if (error) {

        httpError("bind", error);

        return;
    }

    acceptorConnection.listen(net::socket_base::max_listen_connections, error);

    if (error) {

        httpError("listen", error);

        return;
    }
}

HTTPConnection::~HTTPConnection() {

    delete returnResponse;
}


std::string HTTPConnection::errorHTTPConnection() {

    return errorConnection;
}

void HTTPConnection::run() {

    newConnection();
}

void HTTPConnection::newConnection() {

    acceptorConnection.async_accept(net::make_strand(ioContext), beast::bind_front_handler(&HTTPConnection::createSession, shared_from_this()));
}

void HTTPConnection::createSession(beast::error_code error, tcp::socket socket) {

    if (error) {

        httpError("accept", error);

        return;
    }
    else {
;
        std::make_shared<HTTPConnect>(std::move(socket), sslContext, rootDirectory, returnResponse, codWebsite)->run();
    }

    newConnection();
}

void HTTPConnection::httpError(char const* what, beast::error_code& errorMessage) {

    errorConnection = what + ' ' + errorMessage.message();
}