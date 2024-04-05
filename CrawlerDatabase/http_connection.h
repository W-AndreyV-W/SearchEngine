#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
//#include <boost/beast/core.hpp>
//#include <boost/beast/ssl.hpp>
//#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
//#include <boost/config.hpp>

#include "libs/beast/example/common/server_certificate.hpp"
#include "return_response.h"
#include "http_connect.h"

//namespace beast = boost::beast;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection> {

public:

    HTTPConnection(net::io_context& ioc, ssl::context& ctx, tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root, 
                   std::vector<std::string> connectionData, std::string htmlWebsite);
    ~HTTPConnection();

    std::string errorHTTPConnection();
    void run();

private:

    ReturnResponse* returnResponse;

    net::io_context& ioContext;
    ssl::context& sslContext;
    tcp::acceptor acceptorConnection;
    std::shared_ptr<std::string const> rootDirectory;

    std::string errorConnection;
    std::string codWebsite;

    void newConnection();
    void createSession(beast::error_code error, tcp::socket socket);
    void httpError(char const* what, beast::error_code& errorMessage);
};

