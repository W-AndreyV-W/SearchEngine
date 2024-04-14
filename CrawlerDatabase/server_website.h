#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "http_connection.h"

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class ServerWebsite {

public:

    ServerWebsite(std::vector<std::string> connectionDada, std::vector<std::string> connectionDatabase, std::vector<std::string> cert, std::string htmlWebsite);
    void startServer();

private:

    int numberThreads = 1;
    std::string codWebsite;
    std::vector<std::string> connectionDatabase;
    std::vector<std::string> certificate;
    tcp::endpoint* tcpEndpoint = nullptr;
    std::shared_ptr<std::string const>* rootDirectory = nullptr;

    inline void serverCertificate(boost::asio::ssl::context& ctx);
};