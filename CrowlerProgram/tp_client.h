#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
//#include <boost/asio/strand.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
//namespace net = boost::asio;
//using tcp = boost::asio::ip::tcp;

class TPÑlient {

public:

    explicit TPÑlient();
    ~TPÑlient();

    virtual void run(char const* host, char const* port, char const* target, int version) const = 0;
    virtual std::vector<std::string> getRequest();
    virtual bool getError(std::string& errorName);

protected:

    bool errorClient = false;
    std::string nameError;
    std::vector<std::string>* dataWebsite = nullptr;

    virtual void clientError(char const* what, beast::error_code& errorMessage);
    virtual void saveDataWebsite(http::response<http::string_body>& res);
};