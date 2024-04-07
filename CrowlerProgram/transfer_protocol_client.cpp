#include "transfer_protocol_client.h"


std::string TransferProtocol—lient::getError() {

    return nameError;
}

bool TransferProtocol—lient::requestWebsite(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version) {

    if (dataWebsite != nullptr && !requestWeb.empty()) {

        if (requestWeb.at(3) == "http://") {

            return requestHTTP(dataWebsite, requestWeb, version);
        }
    }

    return requestHTTPS(dataWebsite, requestWeb, version);
}

bool TransferProtocol—lient::requestHTTPS(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version) {

    bool error = true;

    try {

        if (!requestWeb.empty()) {

            net::io_context ioContext;
            ssl::context sslContext{ ssl::context::tlsv12_client };
            load_root_certificates(sslContext);
            sslContext.set_verify_mode(ssl::verify_peer);
            std::shared_ptr<HTTPS—lient> https—lient = std::make_shared<HTTPS—lient>(net::make_strand(ioContext), sslContext);
            https—lient->run(requestWeb.at(0).data(), requestWeb.at(1).data(), requestWeb.at(2).data(), version);
            ioContext.run();
            error = https—lient->getHTTPError(nameError);

            if (!error) {

                *dataWebsite = https—lient->getRequest();
            }
        }
    }
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
        error = true;
    }

    return !error;
}

bool TransferProtocol—lient::requestHTTP(std::vector<std::string>* dataWebsite, std::vector<std::string>& requestWeb, int version) {

    bool error = true;

    try {

        if (!requestWeb.at(0).empty()) {

            net::io_context ioContext;
            std::shared_ptr<HTTP—lient> http—lient = std::make_shared<HTTP—lient>(ioContext);
            http—lient->run(requestWeb.at(0).data(), requestWeb.at(1).data(), requestWeb.at(2).data(), version);
            ioContext.run();
            error = http—lient->getHTTPError(nameError);

            if (!error) {

                *dataWebsite = http—lient->getRequest();
            }
        }
    }
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
        error = true;
    }

    return !error;
}