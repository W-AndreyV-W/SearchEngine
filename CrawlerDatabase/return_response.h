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
    std::string linkWebsite1 = " target=";
    std::string linkWebsite2 = "_blank";
    std::string linkWebsite3 = "><b><ins>";
    std::string linkWebsite4 = "</ins></b></a></p>";
    std::string wasNotFound = "<b>Site not found!</b>";

    std::vector<std::string> prepareRequest(std::string request);
    std::vector<std::map<std::string, int>> requestDatabese(std::vector<std::string> request);
    std::multimap<int, std::string, std::greater<int>> websiteSorting(std::vector<std::map<std::string, int>> response);
    std::string responseString(std::multimap<int, std::string, std::greater<int>> requestWebsite);
};


