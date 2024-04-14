#include "certificates_file_open.h"

std::string CertificatesFileOpen::errorFile() {

    return errorFileOpen;
}

std::vector<std::string> CertificatesFileOpen::readingFile(std::string fileName) {

    readFile.open(fileName);
    std::vector<std::string> dataFile;

    if (readFile.is_open()) {

        std::string lineWord;
        std::string word;

        while (std::getline(readFile, word)) {

            if (word.empty()) {

                dataFile.emplace_back(lineWord);
                lineWord = "";
                continue;
            }

            lineWord += word + "\n";
        }

        dataFile.emplace_back(lineWord);
        readFile.close();
    }
    else {

        dataFile.emplace_back("");
        errorFileOpen = "Error opening the website file!";
    }

    return dataFile;
}