#pragma once

#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/locale.hpp>
//#include <boost/url/pct_string_view.hpp>
//#include <boost/url/encoding_opts.hpp>
//#include <boost/url/encode.hpp>
#include <boost/url.hpp>

#include "libs\beast\example\common\root_certificates.hpp"
#include "http_client.h"
#include "https_client.h"

namespace locale = boost::locale;
namespace urls = boost::urls;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

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
    void responseWebsite(int recursion = 0);
    void processingRequest(std::string request);
};