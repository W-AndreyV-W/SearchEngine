#include <iostream>
#include <vector>

#include "server_website.h"
#include "file_open.h"

#pragma execution_character_set( "utf-8" )

int main() {

    SetConsoleOutputCP(CP_UTF8);

	FileOpen fileOpen;
    
    std::vector<std::string> request(7);
	std::vector<std::string> connectionDada(3);

    if (fileOpen.iniParser("../../../program.ini")) {

        request.at(0) = fileOpen.getValue("Section1", "host");
        request.at(1) = fileOpen.getValue("Section1", "port");
        request.at(2) = fileOpen.getValue("Section1", "dbname");
        request.at(3) = fileOpen.getValue("Section1", "user");
        request.at(4) = fileOpen.getValue("Section1", "password");
        request.at(5) = fileOpen.getValue("Section2", "adress");
        request.at(6) = fileOpen.getValue("Section2", "recursion");
        connectionDada.at(0) = fileOpen.getValue("Section3", "adress");
        connectionDada.at(1) = fileOpen.getValue("Section3", "port");
        connectionDada.at(2) = fileOpen.getValue("Section3", "doc_root");        
    }

	ServerWebsite serverWebsite(connectionDada, request, fileOpen.readingFile("../../../html_website.htm"));
	serverWebsite.startServer();

	return 0;
}