#pragma once

#include <iostream>
#include <memory>

#include <boost\asio\strand.hpp>
#include "libs\beast\example\common\root_certificates.hpp"

#include "http_client.h"

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class DownloadWebsite {

public:

    DownloadWebsite();
    ~DownloadWebsite();

    bool download(std::vector<std::string> request);
    std::vector<std::string> getRequest();
    std::string getDownloadError();

private:

    net::io_context* ioContext = nullptr;
    ssl::context* sslContext = nullptr;
    std::shared_ptr<HTTP—lient> http—lient = nullptr;

    int version = 11;
    std::string nameError;
    std::vector<std::string>* requestWeb = nullptr;
    std::vector<std::string>* dataWebsite = nullptr;

    void newContext();
    bool requestWebsite();
    void processingAddress(std::vector<std::string>& request, int i);
};