#include "thread_pool.h"

ThreadPool::ThreadPool(std::vector<std::string> initialData) {

	databaseInit = new std::vector<std::string>(5);
	poolAdress = new std::vector<std::pair<std::string, int>>;
	numberThreads = std::thread::hardware_concurrency();

	databaseInit->push_back(initialData.at(0));
	databaseInit->push_back(initialData.at(1));
	databaseInit->push_back(initialData.at(2));
	databaseInit->push_back(initialData.at(3));
	databaseInit->push_back(initialData.at(4));
	poolAdress->emplace_back(std::pair<std::string, int>(initialData.at(5), std::stoi(initialData.at(6))));
	
	poolScanningWebsite();
}

ThreadPool::~ThreadPool() {

	delete poolThread;
	delete poolAdress;
}

void ThreadPool::poolScanningWebsite() {

	poolThread = new std::vector<std::jthread>(numberThreads);

	for (int i = 0; i < numberThreads; i++) {

		if (i == 0) {

			poolThread->push_back(std::jthread(&ThreadPool::scanningWebsite, this, true));
		}
		else {

			poolThread->push_back(std::jthread(&ThreadPool::scanningWebsite, this, false));
		}
	}
}

void ThreadPool::scanningWebsite(bool createDate) {

	DownloadWebsite* downloadWebsite;
	ProcessingDataSite processingDataSite;
	//DatabaseWebsite databaseWebsite(databaseInit, createDate);
	

	int search = 0;
	std::vector<std::string> request = { {""} };
	std::map<std::string, int> poolAdressFunction;

	while (true) {

		downloadWebsite = new DownloadWebsite;
		
		bool working = readingFromAddressPool(request, search);


		if (working) {

			

			downloadWebsite->download(request);

			processingDataSite.processing(downloadWebsite->getRequest());
;
			//std::thread saveDatabese([]() {
			//	databaseWebsite.writingDatabase(processingDataSite.adressWebsiteInDatabase(), processingDataSite.wordCounInDatabasee()); 
			//	}
			//);

			savingInAddressPool(processingDataSite.adressWebsiteForSearch(), search);

			delete downloadWebsite;
			disablingThreadPool();
			//saveDatabese.join();

		}
		else {
			
			waitingAddress();
		}

		std::unique_lock<std::mutex> lock(block);

		if (!threadIsRunning) {

			break;
		}
	}
}



bool ThreadPool::readingFromAddressPool(std::vector<std::string>& request, int& search) {

	std::unique_lock<std::mutex> lock(block);
	blokThread;

	if (indexPoolAdress < poolAdress->size()) {

		numberRunningThreads++;
		request.at(0) = poolAdress->at(indexPoolAdress).first;
		search = poolAdress->at(indexPoolAdress).second - 1;
		indexPoolAdress++;

		std::cout << poolAdress->size() << " " << indexPoolAdress << " " << request.at(0) << std::endl;

		blokThread;
		return true;
	}

	return false;
}

void ThreadPool::savingInAddressPool(std::set<std::string> adressWebsite, int search) {


	if (search > 0) {
		
		std::unique_lock<std::mutex> lock(block);
		blokThread;

		for (const auto& adressInPool : adressWebsite) {


			for (size_t n = 0; n < poolAdress->size(); n++) {

				if (poolAdress->at(n).first == adressInPool) {

					break;
				}

				if (n == poolAdress->size() - 1) {

					poolAdress->emplace_back(std::pair<std::string, int>(adressInPool, search));

				}
			}
		}

		blokThread;
	}


}

void ThreadPool::disablingThreadPool() {

	std::unique_lock<std::mutex> lock(block);
	numberRunningThreads--;
	
	if (numberRunningThreads < 3 && indexPoolAdress > poolAdress->size() - 1) {
	
		threadIsRunning = false;
	}

	messages.notify_all();
}

void ThreadPool::waitingAddress() {

	std::unique_lock<std::mutex> lock(block);
	
	if (threadIsRunning) {

		messages.wait(lock, [&]() {

				if (indexPoolAdress < poolAdress->size() || !threadIsRunning) {

					return true;
				}
				else {

					return false;
				}
			}
		);
	}
}