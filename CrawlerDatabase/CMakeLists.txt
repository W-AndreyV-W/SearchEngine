cmake_minimum_required(VERSION 3.2)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(PostgreSQL_ADDITIONAL_VERSIONS "16")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CrawlerDatabase)

add_subdirectory("..\\libpqxx-7.9.0" libpqxx-build)

set(BOOST_ROOT C:/boost_1_84_0)

find_package(boost REQUIRED)

set(Boost_NO_SYSTEM_PATHS	  ON)
set(Boost_USE_MULTITHREADED	  ON)
set(Boost_USE_STATIC_LIBS	  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK       OFF)

set(OPENSSL_ROOT_DIR "..\\OpenSSL-Win64")

find_package(OpenSSL REQUIRED)

if(Boost_FOUND)

	add_executable(CrawlerDatabase	crawler_database.cpp 
									database_website.h 
									database_website.cpp
									file_open.h
									file_open.cpp
									certificates_file_open.h
									certificates_file_open.cpp
									http_connect.h
									http_connect.cpp
									http_connection.h
									http_connection.cpp
									return_response.h
									return_response.cpp
									server_website.h
									server_website.cpp
	)

	target_link_libraries(CrawlerDatabase	pqxx
											${Boost_LIBRARIES}
											OpenSSL::SSL
											OpenSSL::Crypto
											OpenSSL::applink
	)

	target_include_directories(CrawlerDatabase PRIVATE ${Boost_INCLUDE_DIRS})

endif()

target_compile_features(CrawlerDatabase PRIVATE cxx_std_20)