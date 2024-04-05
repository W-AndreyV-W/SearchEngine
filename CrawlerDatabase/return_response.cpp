#include "return_response.h"

ReturnResponse::ReturnResponse(std::vector<std::string> connectionData) {

	databaseWebsite = new DatabaseWebsite(connectionData, false);
}

std::string ReturnResponse::responseDatabese(std::string request) {

	std::vector<std::string> preparedRequest = prepareRequest(request);
	std::vector<std::map<std::string, int>> requestData = requestDatabese(preparedRequest);
	std::multimap<int, std::string, std::greater<int>> addresses = websiteSorting(requestData);

	return responseString(addresses);
}

std::vector<std::string> ReturnResponse::prepareRequest(std::string req) {

	size_t indexBegin = 0;
	size_t indexEnd = 0;
	std::string word;
	std::string request;
	std::vector<std::string> response;

	if (!req.empty()) {

		request = urls::pct_string_view(req).decode();

		for (int i = 0; i < 4 && indexBegin != std::string::npos; i++) {

			indexBegin = request.find_first_not_of("+", indexEnd);
			indexEnd = request.find_first_of("+", indexBegin);

			if (indexBegin != std::string::npos) {

				if (indexEnd != std::string::npos) {
				
					word = request.substr(indexBegin, indexEnd - indexBegin);
				}
				else {

					word = request.substr(indexBegin, request.size() - indexBegin);
				}
				
				if (word.size() > 2) {

					response.emplace_back(word);
					std::cout << word << std::endl;
				}
				else {

					i--;
				}
			}
		}
	}

	return response;
}

std::vector<std::map<std::string, int>> ReturnResponse::requestDatabese(std::vector<std::string> request) {

	std::vector<std::map<std::string, int>> response;

	for (const auto& req : request) {

		response.emplace_back(databaseWebsite->requestDatabase(req, 10));
	}

	return response;
}

std::multimap<int, std::string, std::greater<int>> ReturnResponse::websiteSorting(std::vector<std::map<std::string, int>> response) {

	std::multimap<int, std::string, std::greater<int>> requestWebsite;

	if (!response.empty()) {

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

		for (const auto& req : response.at(0)) {

			requestWebsite.emplace(req.second, req.first);
		}
	}

	return requestWebsite;
}

std::string ReturnResponse::responseString(std::multimap<int, std::string, std::greater<int>> requestWebsite) {

	int i = 1;
	std::string linkWebsite = "<hr><hr><hr>";

	if (!requestWebsite.empty()) {

		urls::encoding_opts encodingOpts;
		encodingOpts.disallow_null = true;

		for (const auto& request : requestWebsite) {

			if (i <= 10) {

				std::string requestEncode = urls::encode(request.second, urls::pchars + '/', encodingOpts);
				std::string lineAddress = linkWebsite0 + '"' + requestEncode + '"' + linkWebsite1 + '"' + linkWebsite2 + '"' +
										  linkWebsite3 + std::to_string(i) + ". -  " + request.second + linkWebsite4;
				linkWebsite += lineAddress;
				i++;
			}
			else {

				break;
			}
		}
	}
	else {

		linkWebsite += wasNotFound;
	}

	return linkWebsite;
}