#include "server_website.h"
    
ServerWebsite::ServerWebsite(std::vector<std::string> connectionDada, std::vector<std::string> connectionBase, std::string htmlWebsite) : 
                             connectionDatabase(connectionBase){
    
    tcpEndpoint = new tcp::endpoint{ net::ip::make_address(connectionDada.at(0)), static_cast<unsigned short>(std::stoi(connectionDada.at(1))) };
    rootDirectory = new std::shared_ptr<std::string const>(std::make_shared<std::string>(connectionDada.at(2)));
    numberThreads = std::thread::hardware_concurrency() / 2 + 1;
    codWebsite = htmlWebsite;
}

void ServerWebsite::startServer() {

    net::io_context ioContext{ numberThreads };
    ssl::context sslContext{ ssl::context::tlsv12 };
    load_server_certificate(sslContext);
    std::make_shared<HTTPConnection>(ioContext, sslContext, *tcpEndpoint, *rootDirectory, connectionDatabase, codWebsite)->run();
    std::vector<std::thread> poolThreads(numberThreads - 1);

    for (int i = numberThreads - 1; i > 0; i--) {

        poolThreads.emplace_back([&ioContext] {

            ioContext.run();
            });
    }

    ioContext.run();
}