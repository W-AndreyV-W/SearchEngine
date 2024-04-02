#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

//#pragma comment(lib, "libssl.lib")
//#pragma comment(lib, "libcrypto.lib")
//#pragma comment(lib, "crypt32.lib")

#include "libs/beast/example/common/server_certificate.hpp"
#include "http_connection.h"

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class ServerWebsite {

public:

    ServerWebsite(std::vector<std::string> connectionDada, std::vector<std::string> connectionDatabase, std::string htmlWebsite);
    void startServer();

private:

    int numberThreads = 1;
    std::string codWebsite;
    std::vector<std::string> connectionDatabase;
    tcp::endpoint* tcpEndpoint = nullptr;
    std::shared_ptr<std::string const>* rootDirectory = nullptr;
};

