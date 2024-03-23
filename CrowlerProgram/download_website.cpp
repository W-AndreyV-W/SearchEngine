#include "download_website.h"

DownloadWebsite::DownloadWebsite() {
}

DownloadWebsite::~DownloadWebsite() {

    http—lient.~shared_ptr();
    delete ioContext;
    delete sslContext;
    delete requestWeb;
    delete dataWebsite;
}

bool DownloadWebsite::download(std::vector<std::string> request) {

    requestWeb = new std::vector<std::string>(3);
    requestWeb->at(1) = "443";

    try { 

        processingAddress(request, 0);
        
        if (request.size() > 1 && !request.at(1).empty()) {

            requestWeb->at(1) = request.at(1);
        }

        if (request.size() > 2 && !request.at(2).empty()) {

            version = static_cast<int>(std::stoi(request.at(3)));
        }
    }
    catch (std::exception const& errorMessage) {
        
        nameError = errorMessage.what();

        return false;
    }

    return requestWebsite();
}

std::vector<std::string> DownloadWebsite::getRequest() {

    dataWebsite->at(1) = requestWeb->at(0) + requestWeb->at(2);

    return *dataWebsite;
}

std::string DownloadWebsite::getDownloadError() {

    return nameError;
}

void DownloadWebsite::newContext() {

    ioContext = new net::io_context;
    sslContext = new ssl::context{ ssl::context::tlsv12_client };
    
    load_root_certificates(*sslContext);
    sslContext->set_verify_mode(ssl::verify_peer);

    http—lient = std::make_shared<HTTP—lient>(net::make_strand(*ioContext), *sslContext);
    http—lient->run(requestWeb->at(0).data(), requestWeb->at(1).data(), requestWeb->at(2).data(), version);
    
    ioContext->run();
}

bool DownloadWebsite::requestWebsite() {


    try {

        newContext();


        if (!http—lient->getHTTPError(nameError)) {

            dataWebsite = new std::vector<std::string>(http—lient->getRequest());



            return true;
        }

        dataWebsite = new std::vector<std::string>(3);
        dataWebsite->at(0) = "000";

    } 
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
        return false;
    }

    return false;
}

void DownloadWebsite::processingAddress(std::vector<std::string>& request, int i) {

    size_t index = request.at(i).find("/");

    if (index != std::string::npos) {

        if (index + 1 < request.at(i).size()) {

            if (request.at(i).substr(index, 2) == "//") {

                request.at(i).erase(0, index + 2);
                index = request.at(i).find("/");
            }
        }

        if (index != std::string::npos) {

            requestWeb->at(0) = request.at(i).substr(0, index);
            requestWeb->at(2) = request.at(i).substr(index);
        }
    }
    else {

        requestWeb->at(0) = request.at(i);
        requestWeb->at(2) = "/";
    }

    //std::cout << requestWeb->at(0) << requestWeb->at(2) << std::endl;
}
