cmake_minimum_required(VERSION 3.14)

project(ingestion-client LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Network REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Add Google Test using FetchContent
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add the source files for the main executable
add_executable(ingestion-client
  main.cpp
  NodeRegistration.h
  NodeRegistration.cpp
  FolderDataHandler.h
  FolderDataHandler.cpp
  CSVProcessor.h
  CSVProcessor.cpp
  QueryModule.h
  QueryModule.cpp
  DataStreamer.h
  DataStreamer.cpp
)

target_link_libraries(ingestion-client Qt6::Core Qt6::Network)
target_link_libraries(ingestion-client Qt${QT_VERSION_MAJOR}::Core)

# Add the test executable
add_executable(ingestion-client-tests
  main_test.cpp
  NodeRegistration.h
  NodeRegistration.cpp
  FolderDataHandler.h
  FolderDataHandler.cpp
  CSVProcessor.h
  CSVProcessor.cpp
  QueryModule.h
  QueryModule.cpp
  DataStreamer.h
  DataStreamer.cpp
)

target_link_libraries(ingestion-client-tests Qt6::Core Qt6::Network)
target_link_libraries(ingestion-client-tests Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ingestion-client-tests gtest gtest_main)

include(GNUInstallDirs)
install(TARGETS ingestion-client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Add a custom target to run the tests
add_test(NAME IngestionClientTests COMMAND ingestion-client-tests)
