#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/url.hpp>

#include "database_website.h"

namespace urls = boost::urls;

class ReturnResponse {

public:

    ReturnResponse(std::vector<std::string> connectionData);

    std::string responseDatabese(std::string req);
    
private:

    DatabaseWebsite* databaseWebsite;

    std::string linkWebsite0 = "<p><a href=";
    std::string linkWebsite1 = "><b><ins>";
    std::string linkWebsite2 = "</ins></b></a></p>";

    std::vector<std::map<std::string, int>> requestDatabese(std::string request);
    std::multimap<int, std::string, std::greater<int>> websiteSorting(std::vector<std::map<std::string, int>> response);
    std::string responseString(std::multimap<int, std::string, std::greater<int>> requestWebsite);
};


