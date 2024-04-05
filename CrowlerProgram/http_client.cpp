#include "http_client.h"

HTTP—lient::HTTP—lient(net::io_context& ioc) {

    resolver—lient = new tcp::resolver(net::make_strand(ioc));
    stream—lient = new beast::tcp_stream(net::make_strand(ioc));
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

    if (dataWebsite != nullptr) {

        delete dataWebsite;
    }
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

    stream—lient->expires_after(std::chrono::seconds(30));
    stream—lient->async_connect(results, beast::bind_front_handler(&HTTP—lient::connectWebsite, shared_from_this()));
}

void HTTP—lient::connectWebsite(beast::error_code errorMessage, tcp::resolver::results_type::endpoint_type) {

    if (errorMessage) {

        httpError("Connect", errorMessage);
        return;
    }
    
    stream—lient->expires_after(std::chrono::seconds(30));
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
    errorClient = false;

    stream—lient->socket().shutdown(tcp::socket::shutdown_both, errorMessage);

    if (errorMessage && errorMessage != beast::errc::not_connected) {

        //httpError("Shutdown", errorMessage);
        return;
    }
}

void HTTP—lient::saveDataWebsite() {

    dataWebsite = new std::vector<std::string>(3);

    dataWebsite->at(0) = std::to_string(response—lient->result_int());

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