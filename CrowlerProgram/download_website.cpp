#include "download_website.h"

DownloadWebsite::DownloadWebsite(std::string& cert) {

    transferProtocol—lient = new TransferProtocol—lient(cert);
    requestWeb = new std::vector<std::string>(4);
}

DownloadWebsite::~DownloadWebsite() {
    
    delete transferProtocol—lient;
    delete requestWeb;

    if (dataWebsite != nullptr) {

        delete dataWebsite;
    }
}

bool DownloadWebsite::download(std::vector<std::string> request) {

    try { 

        requestWeb->at(1) = "443";
        requestWeb->at(2) = "/";
        requestWeb->at(3) = "https://";
        processingRequest(request.at(0));

        if (request.size() > 1 && !request.at(1).empty()) {

            version = static_cast<int>(std::stoi(request.at(1)));
        }
        
        responseWebsite();
    }
    catch (std::exception const& errorMessage) {
        
        nameError = errorMessage.what();
        return false;
    }

    return errorHTTP;
}

std::vector<std::string> DownloadWebsite::getRequest() {

    return std::move(*dataWebsite);
}

std::string DownloadWebsite::getDownloadError() {

    return nameError;
}

void DownloadWebsite::responseWebsite(int recursion) {

    try {

        errorHTTP = false;

        if (dataWebsite == nullptr) {

            dataWebsite = new std::vector<std::string>(3);
        }

        if (transferProtocol—lient->requestWebsite(dataWebsite, *requestWeb, version)) {

            if (dataWebsite->at(0) == "200") {

                if (!dataWebsite->at(2).empty()) {

                    errorHTTP = true;
                }

                dataWebsite->at(1) = requestWeb->at(3) + requestWeb->at(0) + requestWeb->at(2);
            }
            else if (!dataWebsite->at(1).empty() && recursion == 0) {

                processingRequest(dataWebsite->at(1));
                responseWebsite(1);
            }
        }
        else {

            dataWebsite->at(0) = "000";
        }
    } 
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
        errorHTTP = false;
    }
}

void DownloadWebsite::processingRequest(std::string request) {

    try {

        transferProtocol(request);
        hostWebsite(request);
        portWebsite(request);
        targetWebsite(request);
    }
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
    }
}

void DownloadWebsite::transferProtocol(std::string& request) {

    if (request.find("https://") != std::string::npos) {

        requestWeb->at(3) = "https://";
    }
    else if (request.find("http://") != std::string::npos) {

        requestWeb->at(3) = "http://";
    }
}

void DownloadWebsite::hostWebsite(std::string& request) {

    size_t index = request.find("//");

    if (index != std::string::npos) {

        index += 2;
    }
    else {

        index = 0;
    }

    size_t indexEnd = request.find_first_of({ ':', '/' }, index);

    if (indexEnd != std::string::npos) {

        requestWeb->at(0) = request.substr(index, indexEnd - index);
    }
    else {

        requestWeb->at(0) = request.substr(index, request.size() - index);
    }
}

void DownloadWebsite::portWebsite(std::string& request) {

    size_t index = request.find("//");

    if (index != std::string::npos) {

        index += 2;
    }
    else {
        
        index = 0;
    }

    index = request.find_first_of({ ':', '/' }, index);

    if (index != std::string::npos && request.substr(index, 1) == ":") {

        index++;
        size_t indexEnd = request.find("/", index);

        if (indexEnd != std::string::npos) {

            requestWeb->at(1) = request.substr(index, indexEnd - index);
        }
        else {
        
            requestWeb->at(1) = request.substr(index, request.size() - index);
        }
    }
    else if (request.find("https://") != std::string::npos) {

        requestWeb->at(1) = "443";
    }
    else if (request.find("http://") != std::string::npos) {

        requestWeb->at(1) = "80";
    }
}

void DownloadWebsite::targetWebsite(std::string& request) {

    size_t index = request.find("//");

    if (index != std::string::npos) {

        index += 2;
    }
    else {

        index = 0;
    }

    index = request.find("/", index);

    if (index != std::string::npos) {

        requestWeb->at(2) = request.substr(index, request.size() - index);
    }
    else {

        requestWeb->at(2) = "/";
    }
}