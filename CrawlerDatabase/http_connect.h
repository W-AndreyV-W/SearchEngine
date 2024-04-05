#pragma once
#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <string>
//#include <string_view>
//#include <vector>
//#include <cstdlib>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
//#include <boost/locale.hpp>
//#include <boost/config.hpp>

#include "libs/beast/example/common/server_certificate.hpp"
#include "return_response.h"

//namespace locale = boost::locale;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class HTTPConnect : public std::enable_shared_from_this<HTTPConnect> {

public:

    HTTPConnect(tcp::socket&& socket, ssl::context& ctx, std::shared_ptr<std::string const> const& doc_root, ReturnResponse* response, std::string htmlWebsite);
    ~HTTPConnect();
    void run();
    std::string errorHTTPConnect();

private:

    ReturnResponse* returnResponse;

    beast::ssl_stream<beast::tcp_stream> streamServer;
    std::shared_ptr<std::string const> rootDirectory;
    beast::flat_buffer bufferServer;
    http::request<http::string_body> requestClient;

    std::string codWebsite;
    std::string errorConnect;

    void sslConfirmation();
    void handshake(beast::error_code error);
    void emptyRequest();
    void readRequest(beast::error_code error, std::size_t bytesTransferred);
    void sendResponse(http::message_generator&& message);
    void writeRequest(bool keepAlive, beast::error_code error, std::size_t bytesTransferred);
    void closeConnect();
    void shutdownConnect(beast::error_code error);
    void httpError(char const* what, beast::error_code& errorMessage);
    template <class Body, class Allocator>
    http::message_generator handleRequest(beast::string_view rootDirectory, http::request<Body, 
                                          http::basic_fields<Allocator>>&& requestClient);
    template <class Body, class Allocator>
    http::message_generator messagesRequest(http::request<Body, http::basic_fields<Allocator>>& requestClient,
                                            http::status statusCode, beast::string_view messagesServer);
};