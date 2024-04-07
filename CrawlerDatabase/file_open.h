#pragma once

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class FileOpen {

public:

    bool iniParser(std::string fileName);
    std::string getValue(std::string section, std::string title);
    std::string errorInit();
    std::string readingFile(std::string fileName);

private:

    std::ifstream readFile;
    std::vector<std::vector<std::string>> initData;
    std::string sectionIni;
    std::string errorLine = "Неправильная запись в строке ";
    std::string errorFile = " !!! ";

    void dataPerser(std::string line, int numberLine);
};