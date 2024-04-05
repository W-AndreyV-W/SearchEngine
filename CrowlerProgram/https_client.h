#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class HTTPS—lient : public std::enable_shared_from_this<HTTPS—lient> {

public:

    explicit HTTPS—lient(net::any_io_executor executor, ssl::context& sslContext);
    ~HTTPS—lient();

    void run(char const* host, char const* port, char const* target, int version);
    std::vector<std::string> getRequest();
    bool getHTTPError(std::string& errorName);

private:

    tcp::resolver* resolver—lient = nullptr;
    beast::ssl_stream<beast::tcp_stream>* stream—lient = nullptr;
    beast::flat_buffer* buffer—lient = nullptr;
    http::request<http::empty_body>* request—lient = nullptr;
    http::response<http::string_body>* response—lient = nullptr;

    bool errorClient = true;
    std::string nameError;
    std::vector<std::string>* dataWebsite = nullptr;

    void httpError(char const* what, beast::error_code& errorMessage);
    void resolveWebsite(beast::error_code errorMessage, tcp::resolver::results_type results);
    void connectWebsite(beast::error_code errorMessage, tcp::resolver::results_type::endpoint_type);
    void handshakeWebsite(beast::error_code errorMessage);
    void writeWebsite(beast::error_code errorMessage, std::size_t bytesTransferred);
    void readWebsite(beast::error_code errorMessage, std::size_t bytesTransferred);
    void shutdownWebsite(beast::error_code errorMessage);
    void saveDataWebsite();
};