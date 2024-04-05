#include "processing_data_site.h"

ProcessingDataSite::ProcessingDataSite() {

	locale::generator generatorLocale;
	std::locale loccaleConversions = generatorLocale(" ");
	std::locale::global(loccaleConversions);
}

ProcessingDataSite::~ProcessingDataSite() {

	delete adressWebsite;
	delete wordCount;
	delete dataWebsite;
}

std::string ProcessingDataSite::adressWebsiteInDatabase() {

	return dataWebsite->at(1);
}

std::map<std::string, int> ProcessingDataSite::wordCounInDatabasee() {

	return *wordCount;
}

std::set<std::string> ProcessingDataSite::adressWebsiteForSearch() {

	return *adressWebsite;
}

void ProcessingDataSite::processing(std::vector<std::string> data) {
	
	delete dataWebsite;
	delete adressWebsite;
	delete wordCount;
	adressWebsite = new std::set<std::string>;
	wordCount = new std::map<std::string, int>;

	if (data.at(0) == "200") {

		dataWebsite = new std::vector<std::string>(data);

		for (size_t index = 0; index < dataWebsite->at(2).size(); index++) {

			if (dataWebsite->at(2).at(index) == '<') {

				if (dataWebsite->at(2).substr(index, 8) == "<a href=") {

					index = addressProcessing(index);
				}
				else if (dataWebsite->at(2).substr(index, 7) == "<script") {

					index = skipCommand(index + 7);
				}
				else if (dataWebsite->at(2).substr(index, 6) == "<style") {

					index = skipCommand(index + 6);
				}
				else if (dataWebsite->at(2).substr(index, 4) == "<!--") {

					index = skipComments(index + 4);
				}
			}
			else if (dataWebsite->at(2).at(index) == '>') {

				index = textProcessing(index);
			}
		}
	}
}

size_t ProcessingDataSite::addressProcessing(size_t index) {
	
	try {

		index += 9;
		size_t indexBegin = index;
		size_t indexEnd = dataWebsite->at(2).find_first_of(separatorsAdress, indexBegin);

		if (indexEnd != std::string::npos && indexBegin != indexEnd) {

			index = indexEnd;


			//if (dataWebsite->at(2).substr(indexEnd - 5, 5) == ".html" || dataWebsite->at(2).substr(indexEnd - 4, 4) == ".php") {

			//	indexEnd = dataWebsite->at(2).rfind('/', indexEnd - 4);
			//}
			if (dataWebsite->at(2).substr(indexBegin, 1) != "#" && dataWebsite->at(2).substr(indexBegin, 4) != "tel:" && 
				dataWebsite->at(2).substr(indexBegin, 7) != "mailto:") {

				std::string adressNormaliz = addressNormalization(dataWebsite->at(2).substr(indexBegin, indexEnd - indexBegin));

				size_t sizeAddress = adressNormaliz.size();
				 //indexAddress = adressNormaliz.find_last_of("/", 0);
				size_t indexAddress = adressNormaliz.rfind("/", sizeAddress - 1);
				//std::cout << adressNormaliz << std::endl;
				if (adressNormaliz != dataWebsite->at(1) && adressNormaliz.find_first_of(".", indexAddress) == std::string::npos) {

					adressWebsite->insert(adressNormaliz);
					//std::cout << adressNormaliz << std::endl;
				}
			}
		}
	}
    catch (std::exception const& errorMessage) {
    }

	return index;
}

std::string ProcessingDataSite::addressNormalization(std::string address) {

	if (address.substr(0, 7) != "http://" && address.substr(0, 8) != "https://") {

		size_t indexAddress = address.find("//", 0);

		if (indexAddress != std::string::npos) {

			address.erase(0, indexAddress + 2);
		}
		else if (address.substr(0, 2) == "./") {

			address = dataWebsite->at(1) + address;
		}
		else if (address.substr(0, 3) == "../") {

			size_t sizeAddress = dataWebsite->at(1).size();

			do {

				address.erase(0, 3);
				indexAddress = dataWebsite->at(1).rfind("/", sizeAddress - 1);

				if (indexAddress != std::string::npos) {

					sizeAddress = indexAddress + 1;
				}

			} while (address.substr(0, 3) == "../");

			address = dataWebsite->at(1).substr(0, sizeAddress) + address;
		}
		else if (address.substr(0, 1) == "/") {

			size_t sizeAddress = dataWebsite->at(1).find('/', 8);
			address = dataWebsite->at(1).substr(0, sizeAddress) + address;
		}
	}

	size_t indexAddress = address.find_first_of({ 0x23, 0x3f }, 0);

	if (indexAddress != std::string::npos) {

			address.erase(indexAddress, address.size() - indexAddress);
	}

	size_t sizeAddress = address.size();

	if (address.substr(sizeAddress - 1, 1) == ".") {

		address.erase(sizeAddress - 1, 1);
	}
	//std::cout << address << std::endl;

	//std::string addressDecode = urls::pct_string_view(address).decode();
	//std::string addressLower = locale::to_lower(addressDecode);
	return address;
}

size_t ProcessingDataSite::textProcessing(size_t index) {

	try {

		size_t indexBegin = 0;
		size_t indexEnd = 0;

		while (true) {

			indexBegin = dataWebsite->at(2).find_first_not_of(separatorsWord, index + 1);

			if (indexBegin == std::string::npos) {

				break;
			}

			if (dataWebsite->at(2).at(indexBegin - 1) == '<') {

				return indexBegin - 2;
			}
			else if (dataWebsite->at(2).substr(indexBegin - 2, 2) == "</") {

				return indexBegin - 3;
			}
			else if (dataWebsite->at(2).substr(indexBegin - 2, 2) == "<!") {

				return indexBegin - 3;
			}
			else if (dataWebsite->at(2).at(indexBegin - 1) == '&' || dataWebsite->at(2).at(indexBegin - 2) == '&') {

				index = skipCharacter(indexBegin);
				continue;
			}
			else if (dataWebsite->at(2).at(indexBegin) >= 0xfffffff0) {

				index += 3;
				continue;
			}
			else if (dataWebsite->at(2).at(indexBegin) >= 0xffffffe0) {

				index += 2;
				continue;
			}
			else if (dataWebsite->at(2).at(indexBegin) >= 0xffffffd2) {

				index++;
				continue;
			}
			else if (dataWebsite->at(2).at(indexBegin) >= 0xffffffc0 && dataWebsite->at(2).at(indexBegin) < 0xffffffd0) {

				index++;
				continue;
			}

			indexEnd = dataWebsite->at(2).find_first_of(separatorsWord, indexBegin + 1);

			if (indexEnd == std::string::npos) {

				break;
			}

			index = indexEnd;
			std::string wordNormalize = wordNormalization(indexBegin, indexEnd);
			int characters = countingCharacters(wordNormalize);
			countingWord(characters, wordNormalize);
		}
	}
    catch (std::exception const& errorMessage) {
    }

	return index;
}

std::string ProcessingDataSite::wordNormalization(size_t indexBegin, size_t indexEnd) {


	bool endToBegin = false;

	do {

		endToBegin = false;

		if (dataWebsite->at(2).at(indexEnd - 4) >= 0xfffffff0) {

			indexEnd -= 4;
			endToBegin = true;
		}
		else if (dataWebsite->at(2).at(indexEnd - 3) >= 0xffffffe0) {

			indexEnd -= 3;
			endToBegin = true;
		}
		else if (dataWebsite->at(2).at(indexEnd - 2) >= 0xffffffd2) {

			indexEnd -= 2;
			endToBegin = true;
		}
		else if (dataWebsite->at(2).at(indexEnd - 2) >= 0xffffffc0 && dataWebsite->at(2).at(indexEnd - 2) < 0xffffffd0) {

			indexEnd -= 2;
			endToBegin = true;
		}

	} while (endToBegin);

	std::string word = dataWebsite->at(2).substr(indexBegin, indexEnd - indexBegin);
	std::string wordNormalize = locale::to_lower(locale::normalize(word));

	return wordNormalize;
}

int ProcessingDataSite::countingCharacters(std::string word) {

	int characters = 0;

	for (size_t n = 0; n < word.size(); n++) {

		if (word.at(n) >= 0xfffffff0) {

			n += 3;
		}
		else if (word.at(n) >= 0xffffffe0) {

			n += 2;
		}
		else if (word.at(n) >= 0xffffffc0) {

			n++;
		}

		characters++;
	}

	return characters;
}

void ProcessingDataSite::countingWord(int characters, std::string word) {

	if (characters > 2 && characters <= 32) {

		auto iteratorWordCount = wordCount->find(word);

		if (iteratorWordCount != wordCount->end()) {

			iteratorWordCount->second++;
		}
		else {

			wordCount->emplace(word, 1);
		}
	}
}

size_t ProcessingDataSite::skipCommand(size_t index) {

	size_t n = index;

	for (; n < dataWebsite->at(2).size(); n++) {

		if (dataWebsite->at(2).at(n) == '<') {

			return n - 1;
		}
	}

	return n;
}

size_t ProcessingDataSite::skipComments(size_t index) {

	size_t n = index;
	
	for (; n < dataWebsite->at(2).size(); n++) {

		if (dataWebsite->at(2).at(n) == '>') {

			if (dataWebsite->at(2).substr(n - 2, 3) == "-->") {

				break;
			}
		}
	}

	return n;
}

size_t ProcessingDataSite::skipCharacter(size_t index) {

	for (size_t n = index; n < dataWebsite->at(2).size(); n++) {

		if (dataWebsite->at(2).at(n) == ';') {

			index = n + 1;
			break;
		}
	}

	return index;
}
