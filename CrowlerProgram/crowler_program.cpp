#include <iostream>
#include <vector>
#include <map>

#include "database_website.h"

//#include "processing_data_site.h"
#include "thread_pool.h"
#include "ini_file_open.h"

#pragma execution_character_set( "utf-8" )

int main(int, char**) {

	std::string error;
	std::vector<std::string> data(5);
	bool createDate = true;
	data[0] = "localhost";
	data[1] = "5432";
	data[2] = "Test1";
	data[3] = "postgres";
	data[4] = "H(jM#1}CP=k{`c$r";

	DatabaseWebsite databaseWebsite(data, createDate);;

	std::map<std::string, int>* hhh;
	
	hhh = new std::map<std::string, int>;
	hhh->emplace("ghuyyg", 12);

	std::string kkk = "ghuyyg";


	bool ppp = databaseWebsite.writingDatabase(kkk, hhh);

	std::map<std::string, int>* lll = databaseWebsite.requestDatabase(kkk);

	for (const auto& jjj : *lll) {

		std::cout << jjj.first << " " << jjj.second << std::endl;
	}



        SetConsoleOutputCP(CP_UTF8);



        IniFileOpen iniFileOpen;
        std::vector<std::string> request(7);

        if (iniFileOpen.iniParser("program.ini")) {

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
