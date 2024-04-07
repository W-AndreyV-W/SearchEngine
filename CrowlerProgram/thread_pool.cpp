#include "thread_pool.h"

ThreadPool::ThreadPool(std::vector<std::string> initialData) {

	databaseInit = new std::vector<std::string>(5);
	poolAdress = new std::vector<std::pair<std::string, int>>;
	numberThreads = std::thread::hardware_concurrency();

	databaseInit->at(0) = initialData.at(0);
	databaseInit->at(1) = initialData.at(1);
	databaseInit->at(2) = initialData.at(2);
	databaseInit->at(3) = initialData.at(3);
	databaseInit->at(4) = initialData.at(4);
	poolAdress->emplace_back(std::pair<std::string, int>(initialData.at(5), std::stoi(initialData.at(6))));

	poolScanningWebsite();
}

ThreadPool::~ThreadPool() {

	delete poolThread;
	delete poolAdress;
}

void ThreadPool::poolScanningWebsite() {

	DatabaseWebsite databaseWebsite(*databaseInit);

	if (databaseWebsite.creatingDatabase()) {

		poolThread = new std::vector<std::jthread>(numberThreads);

		for (int i = 0; i < numberThreads; i++) {

			poolThread->push_back(std::jthread(&ThreadPool::scanningWebsite, this));
		};
	}
}

void ThreadPool::scanningWebsite() {
	
	try {

		DownloadWebsite downloadWebsite;
		ProcessingDataSite processingDataSite;
		std::unique_lock lock(block);
		DatabaseWebsite databaseWebsite(*databaseInit);
		lock.unlock();
		int search = 0;
		std::vector<std::string> request(1);

		while (true) {

			bool working = readingFromAddressPool(request, search);

			if (working) {

				if (downloadWebsite.download(request)) {
					
					processingDataSite.processing(downloadWebsite.getRequest());

					std::jthread saveDatabese([&]() {
						databaseWebsite.writingDatabase(processingDataSite.adressWebsiteInDatabase(), processingDataSite.wordCounInDatabasee());
						}
					);

					savingInAddressPool(processingDataSite.adressWebsiteForSearch(), search);
				}

				disablingThreadPool();
			}
			else {

				waitingAddress();
			}

			std::lock_guard lock(block);

			if (!threadIsRunning) {

				break;
			}
		}
	}
	catch (std::exception const& errorMessage) {
	}
}



bool ThreadPool::readingFromAddressPool(std::vector<std::string>& request, int& search) {

	std::lock_guard lock(block);

	if (indexPoolAdress < poolAdress->size()) {

		numberRunningThreads++;
		request.at(0) = poolAdress->at(indexPoolAdress).first;
		search = poolAdress->at(indexPoolAdress).second - 1;
		indexPoolAdress++;
		//std::cout << poolAdress->size() << " " << indexPoolAdress << " " << request.at(0) << std::endl;
		return true;
	}

	return false;
}

void ThreadPool::savingInAddressPool(std::set<std::string> adressWebsite, int search) {


	if (search > 0) {
		
		std::lock_guard lock(block);

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
	}
}

void ThreadPool::disablingThreadPool() {

	std::lock_guard lock(block);
	numberRunningThreads--;

	if (numberRunningThreads < 1 && indexPoolAdress > poolAdress->size() - 1) {
	
		threadIsRunning = false;
	}

	messages.notify_all();
}

void ThreadPool::waitingAddress() {

	std::unique_lock lock(block);
	
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