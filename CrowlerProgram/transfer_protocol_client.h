#pragma once

#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/locale.hpp>

#include "libs\beast\example\common\root_certificates.hpp"
#include "http_client.h"
#include "https_client.h"

namespace locale = boost::locale;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;


class TransferProtocol—lient {

public:

    std::string getError();
    bool requestWebsite(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version = 11);

private:

    std::string nameError;;

    bool requestHTTPS(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version);
    bool requestHTTP(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version);
};