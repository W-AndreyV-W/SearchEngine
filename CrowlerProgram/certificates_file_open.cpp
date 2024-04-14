#include "certificates_file_open.h"

std::string CertificatesFileOpen::errorFile() {

    return errorFileOpen;
}

std::string CertificatesFileOpen::readingFile(std::string fileName) {

    readFile.open(fileName);
    std::string dataFile = "";

    if (readFile.is_open()) {

        std::string word;

        while (std::getline(readFile, word)) {

            dataFile += word + "\n";
        }

        readFile.close();
    }
    else {

        errorFileOpen = "Error opening the website file!";
    }

    return dataFile;
}