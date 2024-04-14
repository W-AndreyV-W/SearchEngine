#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CertificatesFileOpen {

public:

    std::string errorFile();
    std::vector<std::string> readingFile(std::string fileName);

private:

    std::ifstream readFile;
    std::string errorFileOpen = "";
};