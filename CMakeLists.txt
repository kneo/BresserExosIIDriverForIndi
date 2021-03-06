cmake_minimum_required(VERSION 3.13)
project(BresserExosIIGoToDriverForIndi VERSION 0.818)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(XML_INSTALL_DIR /usr/share/indi)
set(CMAKE_INSTALL_PREFIX /bin)
configure_file(Config.hpp.in Config.hpp)

find_package(Threads REQUIRED)
#find_package(INDI REQUIRED)

include_directories(${PROJECT_BINARY_DIR})

option(USE_CERR_LOGGING "log error messages using std::cerr for debugging" ON)

add_subdirectory(SerialDeviceControl)

add_executable(BresserExosIIGoToDriverForIndiTest Main.cpp TestSerialImplementation.cpp TestDataReceivedCallback.cpp)
set_target_properties(BresserExosIIGoToDriverForIndiTest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(BresserExosIIGoToDriverForIndiTest PUBLIC SerialDeviceControl ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(BresserExosIIGoToDriverForIndiTest PUBLIC
						  "${PROJECT_BINARY_DIR}"
						  "${PROJECT_SOURCE_DIR}/SerialDeviceControl"
						  
						  )

add_executable(BresserExosIIGoToDriver BresserExosIIGoToDriver.cpp IndiSerialWrapper.cpp)
target_link_libraries(BresserExosIIGoToDriver indidriver SerialDeviceControl ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(BresserExosIIGoToDriver PUBLIC
						  "${PROJECT_BINARY_DIR}"
						  "${PROJECT_SOURCE_DIR}/SerialDeviceControl"
						  
						  )
						  
include(GNUInstallDirs)
install(TARGETS BresserExosIIGoToDriver DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES BresserExosIIGoToDriver.xml DESTINATION ${XML_INSTALL_DIR})
