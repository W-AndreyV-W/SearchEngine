#pragma once

#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/locale.hpp>

#include "libs\beast\example\common\root_certificates.hpp"
#include "transfer_protocol_client.h"
#include "http_client.h"
#include "https_client.h"

namespace locale = boost::locale;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

class DownloadWebsite {

public:

    DownloadWebsite(std::string& cert);
    ~DownloadWebsite();

    bool download(std::vector<std::string> request);
    std::vector<std::string> getRequest();
    std::string getDownloadError();

private:

    TransferProtocol—lient* transferProtocol—lient;
    
    int version = 11;
    bool errorHTTP = false;
    std::string nameError;
    std::vector<std::string>* requestWeb = nullptr;
    std::vector<std::string>* dataWebsite = nullptr;
    bool requestWebsite();
    void responseWebsite(int recursion = 0);
    void processingRequest(std::string request);
    void transferProtocol(std::string& request);
    void hostWebsite(std::string& request);
    void portWebsite(std::string& request);
    void targetWebsite(std::string& request);
};