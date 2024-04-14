#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <vector>
#include <map>
#include <boost/url.hpp>

#include "database_website.h"

#include "thread_pool.h"
#include "ini_file_open.h"
#include "certificates_file_open.h"

namespace urls = boost::urls;

int main() {

    SetConsoleOutputCP(CP_UTF8);

    IniFileOpen iniFileOpen;
    CertificatesFileOpen certificatesFileOpen;

    std::vector<std::string> request(7);

    if (iniFileOpen.iniParser("../../../program.ini")) {

        urls::encoding_opts encodingOpts;
        encodingOpts.disallow_null = true;

        request.at(0) = iniFileOpen.getValue("Section1", "host");
        request.at(1) = iniFileOpen.getValue("Section1", "port");
        request.at(2) = iniFileOpen.getValue("Section1", "dbname");
        request.at(3) = iniFileOpen.getValue("Section1", "user");
        request.at(4) = iniFileOpen.getValue("Section1", "password");
        request.at(5) = urls::encode(iniFileOpen.getValue("Section2", "adress"), urls::pchars + '/', encodingOpts);
        request.at(6) = iniFileOpen.getValue("Section2", "recursion");
        std::string certificates = certificatesFileOpen.readingFile("../../../certificates.txt");

        ThreadPool threadPool(request, certificates);
    }
}