#include "tp_client.h"

TP�lient::TP�lient() {
}

TP�lient::~TP�lient() {

    if (dataWebsite != nullptr) {

        delete dataWebsite;
    }
}

std::vector<std::string> TP�lient::getRequest() {

    return *dataWebsite;
}

bool TP�lient::getError(std::string& errorName) {

    errorName = nameError;
    return errorClient;
}

void TP�lient::clientError(char const* what, beast::error_code& errorMessage) {

    errorClient = true;
    nameError = what + ' ' + errorMessage.message();
}

//void TP�lient::run(char const* host, char const* port, char const* target, int version) {
//
//}

void TP�lient::saveDataWebsite(http::response<http::string_body>& res) {

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