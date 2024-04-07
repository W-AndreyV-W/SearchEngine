#include "file_open.h"

bool FileOpen::iniParser(const std::string fileName) {

    readFile.open(fileName);

    if (readFile.is_open()) {

        std::string line;

        for (int numberLine = 1; std::getline(readFile, line); numberLine++) {

            if (line.empty()) {

                continue;
            }

            dataPerser(line, numberLine);
        }

        readFile.close();
    }
    else {

        errorFile = "������ ������� �����!";
        return false;
    }

    return true;
}

std::string FileOpen::getValue(std::string section, std::string title) {

    for (const auto& data : initData) {

        if (data.at(0) == section && data.at(1) == title) {

            return data.at(2);
        }
    }

    return "";
}

std::string FileOpen::errorInit() {

    return errorFile;
}

std::string FileOpen::readingFile(std::string fileName) {

    readFile.open(fileName);
    std::string dataFile = "";

    if (readFile.is_open()) {

        std::string word;

        while (std::getline(readFile, word)) {

            if (word.empty()) {

                continue;
            }

            dataFile += word;
        }

        readFile.close();
    }
    else {

        errorFile = "Error opening the website file!";
    }

    return dataFile;
}

void FileOpen::dataPerser(std::string line, int numberLine) {

    std::string word;
    std::vector<std::string>dataInit(3);
    std::stringstream iniLine(line);

    try {

        for (int numberWord = 1; iniLine >> word; numberWord++) {

            if (word.find_first_of(';') == 0) {

                break;
            }
            if (word.find_first_of('[') == 0 && numberWord == 1) {

                if (word.find_first_of(']') != word.npos) {

                    sectionIni = word.substr(1, word.find_first_of(']') - 1);
                }
                else {

                    sectionIni = word.substr(1, word.size() - 1);
                }

                break;
            }
            else if (word.front() == '=' && numberWord == 1) {

                throw std::invalid_argument(errorLine += std::to_string(numberLine) + ", ��� ����� ����������.");
            }
            else if (word.find_first_of('=') != word.npos && numberWord == 1) {

                if (word.find_first_of('=') == word.size() - 1) {

                    dataInit.at(1) = word.substr(0, word.find_first_of('='));
                }
                else {

                    dataInit.at(1) = word.substr(0, word.find_first_of('='));
                    dataInit.at(2) = word.substr(word.find_first_of('=') + 1, word.size() - word.find_first_of('='));
                }
            }
            else if (word.find_first_of('=') != word.npos && numberWord == 2) {

                dataInit.at(2) = word.substr(word.find_first_of('=') + 1, word.size() - word.find_first_of('='));
            }
            else if (word.front() == '=' && numberWord > 2) {

                throw std::invalid_argument(errorLine += std::to_string(numberLine) + ", ������ � ����������.");
            }
            else if (numberWord == 1) {

                dataInit.at(1) = word;
            }
            else if (word != "=" && numberWord != 1) {

                if (!dataInit.at(1).empty()) {

                    dataInit.at(2) = dataInit.at(1) + " " + word;
                }
                else {

                    dataInit.at(2) = word;
                }
            }
        }
        if (!dataInit.at(1).empty()) {

            dataInit.at(0) = sectionIni;
            initData.emplace_back(dataInit);
        }
    }
    catch (std::invalid_argument& error) {

        errorFile = errorFile + error.what() + " !!! ";
    }
}