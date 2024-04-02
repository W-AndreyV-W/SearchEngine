#include <iostream>
#include <vector>
#include <map>

#include "database_website.h"

#include "thread_pool.h"
#include "ini_file_open.h"

#pragma execution_character_set( "utf-8" )

int main() {

    SetConsoleOutputCP(CP_UTF8);

    IniFileOpen iniFileOpen;
    std::vector<std::string> request(7);

    if (iniFileOpen.iniParser("../../../program.ini")) {

        request.at(0) = iniFileOpen.getValue("Section1", "host");
        request.at(1) = iniFileOpen.getValue("Section1", "port");
        request.at(2) = iniFileOpen.getValue("Section1", "dbname");
        request.at(3) = iniFileOpen.getValue("Section1", "user");
        request.at(4) = iniFileOpen.getValue("Section1", "password");
        request.at(5) = iniFileOpen.getValue("Section2", "adress");
        request.at(6) = iniFileOpen.getValue("Section2", "recursion");
    }

    ThreadPool threadPool(request);
}
