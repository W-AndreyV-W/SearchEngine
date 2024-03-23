#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <map>

#include "download_website.h"
#include "processing_data_site.h"
#include "database_website.h"

class ThreadPool {

public:

	ThreadPool(std::vector<std::string> initialData);
	~ThreadPool();
std::vector<std::pair<std::string, int>>* poolAdress = nullptr;
private:

	int numberThreads = 10;
	int numberRunningThreads = 0;
	size_t indexPoolAdress = 0;
	bool threadIsRunning = true;
	bool blokThread = true;
	std::vector<std::string>* databaseInit = nullptr;
	
	std::vector<std::jthread>* poolThread = nullptr;

	std::mutex block;
	std::condition_variable messages;
	//std::stop_token* stop_work = stopWork->get_token();

	void poolScanningWebsite();
	void scanningWebsite(bool createDate);
	bool readingFromAddressPool(std::vector<std::string>& request, int& search);
	void savingInAddressPool(std::set<std::string> adressWebsite, int search);
	void disablingThreadPool();
	void waitingAddress();
};

