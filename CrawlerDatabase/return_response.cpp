#include "return_response.h"

ReturnResponse::ReturnResponse(std::vector<std::string> connectionData) {

	databaseWebsite = new DatabaseWebsite(connectionData, false);
}

std::string ReturnResponse::responseDatabese(std::string request) {

	return responseString(websiteSorting(requestDatabese(request)));
}

std::vector<std::map<std::string, int>> ReturnResponse::requestDatabese(std::string req) {

	size_t indexBegin = 0;
	size_t indexEnd = 0;
	std::string word;
	std::string request;
	std::vector<std::map<std::string, int>> response;

	request = urls::pct_string_view(req).decode();

	for (int i = 0; i < 4 && indexEnd != std::string::npos; i++) {

		indexEnd = request.find_first_of("+");

		if (indexEnd == std::string::npos) {

			word = request.substr(indexBegin, request.size() - indexBegin);
		}
		else {

			word = request.substr(indexBegin, indexEnd - indexBegin);
			indexBegin = indexEnd + 1;
		}
		
		if (word.size() > 2) {

			response.emplace_back(databaseWebsite->requestDatabase(word, 10));
			i--;
		}
	}

	return response;
}

std::multimap<int, std::string, std::greater<int>> ReturnResponse::websiteSorting(std::vector<std::map<std::string, int>> response) {

	for (size_t i = response.size() - 1; i > 0; i--) {

		for (auto& res : response.at(i)) {

			auto req = response.at(0).find(res.first);

			if (req != response.at(0).end()) {

				req->second += res.second;
			}
			else {

				response.at(0).emplace(res.first, res.second);
			}
		}
	}

	std::multimap<int, std::string, std::greater<int>> requestWebsite;

	for (const auto& req : response.at(0)) {

		requestWebsite.emplace(req.second, req.first);
	}

	return requestWebsite;
}

std::string ReturnResponse::responseString(std::multimap<int, std::string, std::greater<int>> requestWebsite) {

	std::string linkWebsite = "<hr><hr><hr>";
	int i = 0;

	for (const auto& request : requestWebsite) {

		if (i < 10) {

			std::string lineAddress = linkWebsite0 + '"' + request.second + '"' + linkWebsite1 + std::to_string(i + 1) + ". -  " + request.second + linkWebsite2;
			linkWebsite += lineAddress;
			i++;
		}
		else {

			break;
		}
	}

	return linkWebsite;
}

