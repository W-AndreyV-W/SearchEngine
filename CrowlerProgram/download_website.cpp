#include "download_website.h"

DownloadWebsite::DownloadWebsite() {

    requestWeb = new std::vector<std::string>(3);
    requestWeb->at(1) = "443";
}

DownloadWebsite::~DownloadWebsite() {

    delete requestWeb;
    delete dataWebsite;
}

bool DownloadWebsite::download(std::vector<std::string> request) {

    try { 

        processingRequest(request.at(0));

        if (request.size() > 1 && !request.at(1).empty()) {

            version = static_cast<int>(std::stoi(request.at(1)));
        }
    }
    catch (std::exception const& errorMessage) {
        
        nameError = errorMessage.what();

        return false;
    }

    return responseWebsite();
}

std::vector<std::string> DownloadWebsite::getRequest() {

    //dataWebsite->at(1) = requestWeb->at(0) + requestWeb->at(2);

    return *dataWebsite;
}

std::string DownloadWebsite::getDownloadError() {

    return nameError;
}

bool DownloadWebsite::requestWebsite(){
    
    net::io_context ioContext;
    bool error = false;

    if (requestWeb->at(1) == "443") {

        ssl::context sslContext{ ssl::context::tlsv12_client };
        load_root_certificates(sslContext);
        sslContext.set_verify_mode(ssl::verify_peer);

        std::shared_ptr<HTTPS—lient> https—lient = std::make_shared<HTTPS—lient>(net::make_strand(ioContext), sslContext);
        https—lient->run(requestWeb->at(0).data(), requestWeb->at(1).data(), requestWeb->at(2).data(), version);
        ioContext.run();
        error = https—lient->getHTTPError(nameError);

        if (error) {

            if (dataWebsite != nullptr) {

                delete dataWebsite;
            }

            dataWebsite = new std::vector<std::string>(3);

            *dataWebsite = https—lient->getRequest();
        }
    }
    else {

        std::shared_ptr<HTTP—lient> http—lient = std::make_shared<HTTP—lient>(ioContext);
        http—lient->run(requestWeb->at(0).data(), requestWeb->at(1).data(), requestWeb->at(2).data(), version);
        ioContext.run();
        error = http—lient->getHTTPError(nameError);

        if (error) {

            if (dataWebsite != nullptr) {

                delete dataWebsite;
            }

            dataWebsite = new std::vector<std::string>(3);

            *dataWebsite = http—lient->getRequest();
        }
    }
   
    return error;
}

bool DownloadWebsite::responseWebsite(int recursion) {

    errorHTTP = false;

    try {

        if (requestWebsite()) {

            if (dataWebsite->at(0) == "200") {

                errorHTTP = true;

                if (requestWeb->at(1) == "443") {

                    dataWebsite->at(1) = urls::pct_string_view("https://" + requestWeb->at(0) + requestWeb->at(2)).decode();
                }
                else {

                    dataWebsite->at(1) = urls::pct_string_view("http://" + requestWeb->at(0) + requestWeb->at(2)).decode();
                }
            }
            else if  (!dataWebsite->at(1).empty() && recursion == 0){

                processingRequest(dataWebsite->at(1));
                responseWebsite(1);
            }
        }
        else {

            nameError = dataWebsite->at(0);
            dataWebsite->at(0) = "000";
        }
    } 
    catch (std::exception const& errorMessage) {

        nameError = errorMessage.what();
        return false;
    }
    //std::cout << dataWebsite->at(2) << std::endl;
    return errorHTTP;
}

void DownloadWebsite::processingRequest(std::string& request) {

    if (request.substr(0, 7) == "http://") {

        requestWeb->at(1) = "80";
    }

    if (request.substr(0, 8) == "https://") {

        requestWeb->at(1) = "443";
    }

    size_t index = request.find("/");

    if (index != std::string::npos) {

        if (index + 1 < request.size()) {

            if (request.substr(index, 2) == "//") {

                request.erase(0, index + 2);
                index = request.find("/");
            }
        }

        if (index != std::string::npos) {

            requestWeb->at(0) = request.substr(0, index);
            requestWeb->at(2) = request.substr(index);
        }
    }
    else {

        requestWeb->at(0) = request;
        requestWeb->at(2) = "/";
    }
    //std::cout << requestWeb->at(0) << requestWeb->at(2) << std::endl;
}
