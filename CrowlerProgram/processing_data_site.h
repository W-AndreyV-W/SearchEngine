#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <boost/locale.hpp>
#include <boost/url.hpp>

#pragma execution_character_set( "utf-8" )

namespace urls = boost::urls;
namespace locale = boost::locale;

class ProcessingDataSite {

public:

	ProcessingDataSite();
	~ProcessingDataSite();

	std::string adressWebsiteInDatabase();
	std::map<std::string, int> wordCounInDatabasee();
	std::set<std::string> adressWebsiteForSearch();
	void processing(std::vector<std::string> data);

private:

	std::set<std::string>* adressWebsite = nullptr;
	std::map<std::string, int>* wordCount = nullptr;
	std::vector<std::string>* dataWebsite = nullptr;

	const std::string separatorsAdress{ 0x20, 0x22, 0x27, 0x3c, 0x3e, };

	size_t addressProcessing(size_t index);
	std::string addressNormalization(std::string address);
	std::string addressDecode(std::string address);
	size_t textProcessing(size_t index);
	std::string textNormalization(size_t& index);
	std::string wordNormalization(std::string);
	int countingCharacters(std::string word);
	void countingWord(int characters, std::string word);
	size_t skipCommand(size_t index);
	size_t skipComments(size_t index);
	size_t skipCharacter(size_t index, size_t indexEnd);
};