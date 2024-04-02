#pragma once

#include <iostream>
#include <memory>
#include <boost\asio\strand.hpp>
#include <boost/url/pct_string_view.hpp>

#include "libs\beast\example\common\root_certificates.hpp"
#include "http_client.h"
#include "https_client.h"

namespace urls = boost::urls;
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
    
    int version = 11;
    bool errorHTTP = false;
    std::string nameError;
    std::vector<std::string>* requestWeb = nullptr;
    std::vector<std::string>* dataWebsite = nullptr;
    bool requestWebsite();
    bool responseWebsite(int recursion = 0);
    void processingRequest(std::string& request);
};