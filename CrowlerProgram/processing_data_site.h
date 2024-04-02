#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <boost/locale.hpp>

#pragma execution_character_set( "utf-8" )

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
	const std::string separatorsWord{  0x9,  0xa,  0xb,  0xc,  0xd,  0xe, 0x20, 0x21, 0x22, 0x24,
									  0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2e, 0x2f,
									  0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x5b, 0x5c, 0x5d, 0x5f,
									  0x60, 0x7b, 0x7c, 0x7d, 0x7e };   
	const std::string separatorsAdress{ 0x20, 0x22, 0x26, 0x3c, 0x3e, 0x3f };

	size_t addressProcessing(size_t index);
	void addressNormalization(std::string adress);
	size_t textProcessing(size_t index);
	int countingCharacters(std::string word);
	void countingWord(int characters, std::string word);
	size_t skipCommand(size_t index);
	size_t skipComments(size_t index);
	size_t skipCharacter(size_t index);
};
