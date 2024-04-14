#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class CertificatesFileOpen {

public:

    std::string errorFile();
    std::string readingFile(std::string fileName);

private:

    std::ifstream readFile;
    std::string errorFileOpen = "";
};