#pragma once

#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/locale.hpp>

#include "http_client.h"
#include "https_client.h"

namespace locale = boost::locale;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;


class TransferProtocol—lient {

public:

    TransferProtocol—lient(std::string& cert);

    std::string getError();
    bool requestWebsite(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version = 11);

private:

    std::string nameError;
    std::string certificates;

    bool requestHTTPS(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version);
    bool requestHTTP(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version);
    inline void loadCertificates(ssl::context& ctx);
};