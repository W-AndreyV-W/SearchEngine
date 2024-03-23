#include "http_client.h"

HTTP—lient::HTTP—lient(net::any_io_executor executor, ssl::context& sslContext) {
    
    resolver—lient = new tcp::resolver(executor);
    stream—lient = new beast::ssl_stream<beast::tcp_stream>(executor, sslContext);
    buffer—lient = new beast::flat_buffer;
    request—lient = new http::request<http::empty_body>;
    response—lient = new http::response<http::string_body>;
}

HTTP—lient::~HTTP—lient() {

    delete resolver—lient;
    delete stream—lient;
    delete buffer—lient;
    delete request—lient;
    delete response—lient;
    delete dataWebsite;
}

std::vector<std::string> HTTP—lient::getRequest() {

    return *dataWebsite;
}

bool HTTP—lient::getHTTPError(std::string& errorName) {

    errorName = nameError;

    return errorClient; 
}

void HTTP—lient::httpError(char const* what, beast::error_code& errorMessage) {

    errorClient = true;

    nameError = what + ' ' + errorMessage.message();
}

void HTTP—lient::run(char const* host, char const* port, char const* target, int version) {

    if (!SSL_set_tlsext_host_name(stream—lient->native_handle(), host)) {

        beast::error_code errorMessage{ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };

        httpError("Error TLS ext host nane", errorMessage);

        return;
    }

    request—lient->version(version);
    request—lient->method(http::verb::get);
    request—lient->target(target);
    request—lient->set(http::field::host, host);
    request—lient->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    resolver—lient->async_resolve(host, port, beast::bind_front_handler(&HTTP—lient::resolveWebsite, shared_from_this()));
}

void HTTP—lient::resolveWebsite(beast::error_code errorMessage, tcp::resolver::results_type results) {

    if (errorMessage) {

        httpError("Resolve", errorMessage);

        return;
    }

    beast::get_lowest_layer(*stream—lient).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(*stream—lient).async_connect(results, beast::bind_front_handler(&HTTP—lient::connectWebsite, shared_from_this()));
}

void HTTP—lient::connectWebsite(beast::error_code errorMessage, tcp::resolver::results_type::endpoint_type) {

    if (errorMessage) {

        httpError("Connect", errorMessage);

        return;
    }

    stream—lient->async_handshake(ssl::stream_base::client, beast::bind_front_handler(&HTTP—lient::handshakeWebsite, shared_from_this()));
}

void HTTP—lient::handshakeWebsite(beast::error_code errorMessage) {

    if (errorMessage) {

        httpError("Handshake", errorMessage);

        return;
    }
 
    beast::get_lowest_layer(*stream—lient).expires_after(std::chrono::seconds(30));
    http::async_write(*stream—lient, *request—lient, beast::bind_front_handler(&HTTP—lient::writeWebsite, shared_from_this()));
}

void HTTP—lient::writeWebsite(beast::error_code errorMessage, std::size_t bytesTransferred) {

    boost::ignore_unused(bytesTransferred);
    
    if (errorMessage) {

        httpError("Write", errorMessage);

        return;
    }

    http::async_read(*stream—lient, *buffer—lient, *response—lient, beast::bind_front_handler(&HTTP—lient::readWebsite, shared_from_this()));
}

void HTTP—lient::readWebsite(beast::error_code errorMessage, std::size_t bytesTransferred) {

    boost::ignore_unused(bytesTransferred);

    if (errorMessage) {

        httpError("Read", errorMessage);
        return;
    }

    saveDataWebsite();

    beast::get_lowest_layer(*stream—lient).expires_after(std::chrono::seconds(30));
    stream—lient->async_shutdown(beast::bind_front_handler(&HTTP—lient::shutdownWebsite, shared_from_this()));
}

void HTTP—lient::shutdownWebsite(beast::error_code errorMessage) {

    if (errorMessage == net::error::eof) {


        httpError("Shutdown", errorMessage);
    }        
}

void HTTP—lient::saveDataWebsite() {

    dataWebsite = new std::vector<std::string>(3);

    //std::cout << *response—lient << std::endl;

    dataWebsite->at(0) = std::to_string(response—lient->result_int());
    dataWebsite->at(1) = " ";

    if (response—lient->result_int() == 200) {

        dataWebsite->at(2) = response—lient->body();
    }
    else {

        for (const auto& dataWeb : *response—lient) {

            if (dataWeb.name_string() == "location" || dataWeb.name_string() == "Location") {

                dataWebsite->at(1) = dataWeb.value();
            }
        }
    }
}
