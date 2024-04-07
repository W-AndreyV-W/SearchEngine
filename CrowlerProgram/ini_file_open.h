#pragma once

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class IniFileOpen {

public:

    bool iniParser(std::string filename);
    std::string getValue(std::string section, std::string title);
    std::string errorInit();

private:

    std::ifstream iniFile;
    std::vector<std::vector<std::string>> initData;
    std::string sectionIni;
    std::string errorLine = "Неправильная запись в строке ";
    std::string errorFile = " !!! ";

    void dataPerser(std::string line, int numberLine);
};