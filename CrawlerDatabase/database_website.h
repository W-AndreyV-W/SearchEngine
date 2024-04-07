#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <pqxx/pqxx>

class DatabaseWebsite {

public:

    DatabaseWebsite(std::vector<std::string> connectionData, bool createDate);
    ~DatabaseWebsite();

    std::map<std::string, int> requestDatabase(std::string request, int numberLines = 20);

private:

    pqxx::connection* sql;

    std::string selectData1 = "SELECT website_name, qw.quantity "
                              "FROM website ws "
                              "JOIN quantityword qw ON ws.id_website = qw.website_id "
                              "JOIN word wd ON qw.word_id = wd.id_word "
                              "WHERE wd.word_website LIKE '";
    std::string selectData2 = "' ORDER BY qw.quantity DESC "
                              "LIMIT '";
    std::string selectData3 = "'";

    void connectDatabase(std::vector<std::string> connectionData, bool createDate);
};