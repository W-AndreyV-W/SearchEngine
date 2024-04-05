#include "tp_client.h"

TPÑlient::TPÑlient() {
}

TPÑlient::~TPÑlient() {

    if (dataWebsite != nullptr) {

        delete dataWebsite;
    }
}

std::vector<std::string> TPÑlient::getRequest() {

    return *dataWebsite;
}

bool TPÑlient::getError(std::string& errorName) {

    errorName = nameError;
    return errorClient;
}

void TPÑlient::clientError(char const* what, beast::error_code& errorMessage) {

    errorClient = true;
    nameError = what + ' ' + errorMessage.message();
}

//void TPÑlient::run(char const* host, char const* port, char const* target, int version) {
//
//}

void TPÑlient::saveDataWebsite(http::response<http::string_body>& res) {

    dataWebsite = new std::vector<std::string>(3);

    dataWebsite->at(0) = std::to_string(res.result_int());

    if (res.result_int() == 200) {

        dataWebsite->at(2) = res.body();
    }
    else {

        for (const auto& dataWeb : res) {

            if (dataWeb.name_string() == "location" || dataWeb.name_string() == "Location") {

                dataWebsite->at(1) = dataWeb.value();
            }
        }
    }
}