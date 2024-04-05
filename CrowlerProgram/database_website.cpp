#include "database_website.h"

DatabaseWebsite::DatabaseWebsite(std::vector<std::string> connectionData) {

    connectDatabase(connectionData);
}

DatabaseWebsite::~DatabaseWebsite() {

    delete sql;
}

void DatabaseWebsite::connectDatabase(std::vector<std::string> connectionData) {

    try {

        if (!connectionData.empty()) {

            std::string sqlConnectionData = "host=" + connectionData[0] +
                " port=" + connectionData[1] +
                " dbname=" + connectionData[2] +
                " user=" + connectionData[3] +
                " password=" + connectionData[4];

            sql = new pqxx::connection(sqlConnectionData);
        }
    }
    catch (std::exception& error) {
    }
}

std::map<std::string, int> DatabaseWebsite::requestDatabase(std::string request, int numberLines) {

    std::map<std::string, int> answer;

    try {

        if (!request.empty()) {

            pqxx::work selectSQL{ *sql };
            std::string lines = std::to_string(numberLines);

            for (const auto& [website, number] : selectSQL.stream<std::string, int>(selectData1 + selectSQL.esc(request) +
                selectData2 + lines + selectData3)) {

                answer.emplace(website, number);
            }

            selectSQL.commit();
        }
    }
    catch (std::exception& error) {
    }

    return answer;
}

bool DatabaseWebsite::writingDatabase(std::string address, std::map<std::string, int> searchData) {

    bool noError = true;

    try {

        if (!address.empty() && !searchData.empty()) {

            pqxx::work insertSQL{ *sql };

            for (const auto& data : searchData) {

                std::string simNumber = std::to_string(data.second);
                pqxx::result rows = insertSQL.exec(insertData1 + insertSQL.esc(address) +
                                    insertData2 + insertSQL.esc(data.first) +
                                    insertData3 + insertSQL.esc(simNumber) +
                                    insertData4);

                if (rows.affected_rows() == 0) {

                    noError = false;
                }
            }

            insertSQL.commit();
        }
    }
    catch (std::exception& error) {

        noError = false;
    }

    return noError;
}

bool DatabaseWebsite::creatingDatabase() {

    try {

        pqxx::work createSQL{ *sql };
        createSQL.exec(createTableIfNotExists);
        createSQL.commit();
    }
    catch (std::exception& error) {

        return false;
    }

    return true;
}
