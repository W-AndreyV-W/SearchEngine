#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <pqxx/pqxx>

class DatabaseWebsite {

public:

    DatabaseWebsite(std::vector<std::string> connectionData);
    ~DatabaseWebsite();


    std::map<std::string, int> requestDatabase(std::string request, int numberLines = 20);
    bool writingDatabase(std::string adress, std::map<std::string, int> searchData);
    bool creatingDatabase();

private:

    pqxx::connection* sql;

    std::string createTableIfNotExists =    "CREATE TABLE IF NOT EXISTS website ("
                                            "id_website serial8 PRIMARY KEY, "
                                            "website_name varchar(1024) NOT NULL, "
                                            "CONSTRAINT website_unique UNIQUE(website_name)); "

                                            "CREATE TABLE IF NOT EXISTS word ("
                                            "id_word serial8 PRIMARY KEY, "
                                            "word_website varchar(32) NOT NULL, "
                                            "CONSTRAINT word_unique UNIQUE(word_website)); "

                                            "CREATE TABLE IF NOT EXISTS quantityword ("
                                            "id_quantity serial8 PRIMARY KEY, "
                                            "word_id int4 NOT NULL, "
                                            "website_id int4 NOT NULL, "
                                            "quantity int4 NOT NULL, "
                                            "CONSTRAINT quantityword_unique UNIQUE(word_id, website_id), "
                                            "CONSTRAINT quantityword_website_fk FOREIGN KEY(website_id) REFERENCES website(id_website), "
                                            "CONSTRAINT quantityword_word_fk FOREIGN KEY(word_id) REFERENCES word(id_word))";

    std::string insertData1 =   "WITH key_website AS "
                                "(INSERT INTO website (website_name) VALUES ('";
    std::string insertData2 =   "') "
                                "ON CONFLICT ON CONSTRAINT website_unique DO UPDATE "
                                "SET website_name = EXCLUDED.website_name "
                                "RETURNING id_website AS id_web), "
                                "key_word AS "
                                "(INSERT INTO word (word_website) VALUES ('";
    std::string insertData3 =   "') "
                                "ON CONFLICT ON CONSTRAINT word_unique DO UPDATE "
                                "SET word_website = EXCLUDED.word_website "
                                "RETURNING id_word AS id_wor) "
                                "INSERT INTO quantityword (word_id, website_id, quantity) "
                                "SELECT key_word.id_wor, key_website.id_web, ";
    std::string insertData4 =   " "
                                "FROM key_word, key_website "
                                "ON CONFLICT ON CONSTRAINT quantityword_unique DO UPDATE "
                                "SET quantity = EXCLUDED.quantity;";

    void connectDatabase(std::vector<std::string> connectionData);
};