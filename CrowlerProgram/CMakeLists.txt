cmake_minimum_required(VERSION 3.2)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(PostgreSQL_ADDITIONAL_VERSIONS "16")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CrawlerProgram)

add_subdirectory("..\\libpqxx-7.9.0" libpqxx-build)

set(BOOST_ROOT "C:\\boost_1_84_0")

find_package(boost REQUIRED)

set(Boost_NO_SYSTEM_PATHS	  ON)
set(Boost_USE_MULTITHREADED	  ON)
set(Boost_USE_STATIC_LIBS	  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK       OFF)

set(OPENSSL_ROOT_DIR "..\\OpenSSL-Win64")

find_package(OpenSSL REQUIRED)

if(Boost_FOUND)

	add_executable(CrawlerProgram	crawler_program.cpp 
									database_website.h 
									database_website.cpp
									download_website.h
									download_website.cpp
									http_client.h
									http_client.cpp
									https_client.h
									https_client.cpp
									ini_file_open.h
									ini_file_open.cpp
									processing_data_site.h
									processing_data_site.cpp
									thread_pool.h
									thread_pool.cpp
	  )

	target_link_libraries(CrawlerProgram	pqxx
											${Boost_LIBRARIES}
											OpenSSL::SSL
											OpenSSL::Crypto
											OpenSSL::applink
	)

	target_include_directories(CrawlerProgram PRIVATE ${Boost_INCLUDE_DIRS})

endif()

target_compile_features(CrawlerProgram PRIVATE cxx_std_20)
















#cmake_minimum_required(VERSION 3.2)

#set(Postgre_ADDITIONAL_VERSIONS "16")
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#project(Crowler)

#add_subdirectory("..\\libpqxx-7.9.0" libpqxx-build)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(BOOST_ROOT D:/Search/boost)

#find_package(boost REQUIRED) 

#set(Boost_NO_SYSTEM_PATHS	  ON)
#set(Boost_USE_MULTITHREADED	  ON)
#set(Boost_USE_STATIC_LIBS	  ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(BOOST_ALL_DYN_LINK       OFF)

#find_package(OpenSSL REQUIRED)

#set(INC_DIR ../OpenSSL-Win64/include)
#set(LINK_DIR ../OpenSSL-Win64/lib/VC/x64/MDd)

#include_directories(${INC_DIR})
#link_directories(${LINK_DIR})
#link_libraries(OpenSSL	../OpenSSL-Win64/lib/VC/x64/MDd/libcrypto.
#						../OpenSSL-Win64/lib/VC/x64/MDd/libssl.
#)
#
#if(Boost_FOUND)

#	include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
#	add_executable(Crowler	crowler_program.cpp 
#							database_website.h 
#							database_website.cpp
#							download_website.h
#							download_website.cpp
#							http_client.h
#							http_client.cpp
#							ini_file_open.h
#							ini_file_open.cpp
#							processing_data_site.h
#							processing_data_site.cpp
#							thread_pool.h
#							thread_pool.cpp
#	)
#	target_link_libraries(Crowler	pqxx
#									boost
#									OpenSSL
									#OpenSSL::SSL
									#OpenSSL::Crypto
									#OpenSSL::applink
#	)
#endif()

#target_compile_features(Crowler PRIVATE cxx_std_20)