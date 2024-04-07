#include "database_website.h"

DatabaseWebsite::DatabaseWebsite(std::vector<std::string> connectionData, bool createDate) {

    connectDatabase(connectionData, createDate);
}

DatabaseWebsite::~DatabaseWebsite() {

    delete sql;
}

void DatabaseWebsite::connectDatabase(std::vector<std::string> connectionData, bool createDate) {

    try {

        std::string sqlConnectionData = "host=" + connectionData[0] +
                                        " port=" + connectionData[1] +
                                        " dbname=" + connectionData[2] +
                                        " user=" + connectionData[3] +
                                        " password=" + connectionData[4];

        sql = new pqxx::connection(sqlConnectionData);
    }
    catch (std::exception& error) {
    }
}

std::map<std::string, int> DatabaseWebsite::requestDatabase(std::string request, int numberLines) {

    std::map<std::string, int> answer;

    try {

        pqxx::work selectSQL{ *sql };
        std::string lines = std::to_string(numberLines);

        for (const auto& [website, number] : selectSQL.stream<std::string, int>(selectData1 + selectSQL.esc(request) +
                                                                                selectData2 + lines + selectData3)) {

            answer.emplace(website, number);
        }

        selectSQL.commit();
    }
    catch (std::exception& error) {
    }

    return std::move(answer);
}
