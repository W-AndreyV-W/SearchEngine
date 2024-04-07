#include "processing_data_site.h"

ProcessingDataSite::ProcessingDataSite() {

	locale::generator generatorLocale;
	std::locale loccaleConversions = generatorLocale(" ");
	std::locale::global(loccaleConversions);
}

ProcessingDataSite::~ProcessingDataSite() {

	if (dataWebsite != nullptr) {

		delete dataWebsite;
	}

	if (adressWebsite != nullptr) {

		delete adressWebsite;
	}

	if (wordCount != nullptr) {

		delete wordCount;
	}
}

std::string ProcessingDataSite::adressWebsiteInDatabase() {

	return std::move(dataWebsite->at(1));
}

std::map<std::string, int> ProcessingDataSite::wordCounInDatabasee() {

	return std::move(*wordCount);
}

std::set<std::string> ProcessingDataSite::adressWebsiteForSearch() {

	return std::move(*adressWebsite);
}

void ProcessingDataSite::processing(std::vector<std::string> data) {

	if (dataWebsite != nullptr) {

		delete dataWebsite;
	}

	if (adressWebsite == nullptr) {

		adressWebsite = new std::set<std::string>;
	}
	else {

		adressWebsite->clear();
	}

	if (wordCount == nullptr) {

		wordCount = new std::map<std::string, int>;
	}
	else {

		wordCount->clear();
	}

	if (data.at(0) == "200" && !data.at(2).empty()) {

		dataWebsite = new std::vector<std::string>(std::move(data));

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

		std::string addDec = addressDecode(dataWebsite->at(1));
		dataWebsite->at(1) = addDec;
	}
}

size_t ProcessingDataSite::addressProcessing(size_t index) {
	
	try {

		index += 9;
		size_t indexBegin = index;
		size_t indexEnd = dataWebsite->at(2).find_first_of(separatorsAdress, indexBegin);

		if (indexEnd != std::string::npos && indexBegin != indexEnd) {

			index = indexEnd;

			if (dataWebsite->at(2).substr(indexBegin, 1) != "#" && dataWebsite->at(2).substr(indexBegin, 4) != "tel:" && 
				dataWebsite->at(2).substr(indexBegin, 7) != "mailto:") {

				std::string adressNormaliz = addressNormalization(dataWebsite->at(2).substr(indexBegin, indexEnd - indexBegin));
				size_t sizeAddress = adressNormaliz.size();
				size_t indexAddress = adressNormaliz.rfind("/", sizeAddress - 1);

				if (adressNormaliz != dataWebsite->at(1) && adressNormaliz.find_first_of(".", indexAddress) == std::string::npos) {

					adressWebsite->insert(adressNormaliz);
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

	return address;
}

std::string ProcessingDataSite::addressDecode(std::string address) {

	return urls::pct_string_view(address).decode();
}

size_t ProcessingDataSite::textProcessing(size_t index) {

	try {

		size_t indexBegin = 0;
		size_t indexEnd = 0;
		std::string textNormalized = textNormalization(index);

		while (true) {

			indexBegin = textNormalized.find_first_not_of(' ', indexEnd);

			if (indexBegin != std::string::npos) {
				
				std::string word;
				indexEnd = textNormalized.find_first_of(' ', indexBegin);

				if (indexEnd != std::string::npos) {
					
					word = textNormalized.substr(indexBegin, indexEnd - indexBegin);
				}
				else {

					word = textNormalized.substr(indexBegin, textNormalized.size() - indexBegin);
				}

				std::string wordNormalized = wordNormalization(word);
				int characters = countingCharacters(wordNormalized);
				countingWord(characters, wordNormalized);
			}
			else {

				break;
			}
		}
	}
    catch (std::exception const& errorMessage) {
    }

	return index;
}

std::string ProcessingDataSite::textNormalization(size_t& index) {

	std::string lineWords;

	for (size_t i = ++index; i < dataWebsite->at(2).size(); i++) {

		if (dataWebsite->at(2).at(i) != '<') {

			if (dataWebsite->at(2).at(i) == '&') {

				i = skipCharacter(i, dataWebsite->at(2).size());
				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) == 0xffffffcd && dataWebsite->at(2).at(i + 1) < 0xffffffb0) {

				i++;
				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) == 0xffffffcb || dataWebsite->at(2).at(i) == 0xffffffcc) {

				i++;
				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) == 0xffffffca && dataWebsite->at(2).at(i + 1) > 0xffffffb0) {

				i++;
				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) == 0xffffffc2) {

				i++;
				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) > 0x7a && dataWebsite->at(2).at(i) < 0x80) {

				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) > 0x5a && dataWebsite->at(2).at(i) < 0x61) {

				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) > 0x39 && dataWebsite->at(2).at(i) < 0x41) {

				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) > 0x20 && dataWebsite->at(2).at(i) < 0x30) {

				lineWords.push_back(' ');
				continue;
			}
			else if (dataWebsite->at(2).at(i) > 0x8 && dataWebsite->at(2).at(i) < 0x10) {

				lineWords.push_back(' ');
				continue;
			}

			lineWords.push_back(dataWebsite->at(2).at(i));
			
		}
		else {

			index = --i;
			break;
		}
	}

	return lineWords;
}

std::string ProcessingDataSite::wordNormalization(std::string word) {

	std::string wordNormalized = locale::normalize(word);
	std::string wordLower = locale::to_lower(wordNormalized);
	return wordLower;
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

size_t ProcessingDataSite::skipCharacter(size_t index, size_t indexEnd) {

	for (size_t n = index; n < indexEnd; n++) {

		if (dataWebsite->at(2).at(n) == ';') {

			index = n;
			break;
		}
	}

	return index;
}