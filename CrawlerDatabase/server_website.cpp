#include "server_website.h"
    
ServerWebsite::ServerWebsite(std::vector<std::string> connectionDada, std::vector<std::string> connectionBase, std::vector<std::string> cert, 
                             std::string htmlWebsite) : connectionDatabase(connectionBase), certificate(cert) {
    
    tcpEndpoint = new tcp::endpoint{ net::ip::make_address(connectionDada.at(0)), static_cast<unsigned short>(std::stoi(connectionDada.at(1))) };
    rootDirectory = new std::shared_ptr<std::string const>(std::make_shared<std::string>(connectionDada.at(2)));
    numberThreads = std::thread::hardware_concurrency() / 2 + 1;
    codWebsite = htmlWebsite;
}

void ServerWebsite::startServer() {

    net::io_context ioContext{ numberThreads };
    ssl::context sslContext{ ssl::context::tlsv12 };
    serverCertificate(sslContext);
    std::make_shared<HTTPConnection>(ioContext, sslContext, *tcpEndpoint, *rootDirectory, connectionDatabase, codWebsite)->run();
    std::vector<std::thread> poolThreads(numberThreads - 1);

    for (int i = numberThreads - 1; i > 0; i--) {

        poolThreads.emplace_back([&ioContext] {

            ioContext.run();
            });
    }

    ioContext.run();
}

inline void ServerWebsite::serverCertificate(boost::asio::ssl::context& ctx) {

    ctx.set_password_callback([](std::size_t, boost::asio::ssl::context_base::password_purpose) {

            return "localhost";
        });

    ctx.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 |
                    boost::asio::ssl::context::single_dh_use);
    ctx.use_certificate_chain(boost::asio::buffer(certificate.at(0).data(), certificate.at(0).size()));
    ctx.use_private_key(boost::asio::buffer(certificate.at(1).data(), certificate.at(1).size()), boost::asio::ssl::context::file_format::pem);
    ctx.use_tmp_dh(boost::asio::buffer(certificate.at(2).data(), certificate.at(2).size()));
}