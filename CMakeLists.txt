cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

project(InfiniBandClientProject LANGUAGES CXX)

add_executable(InfiniBandClient main.cpp InfinibandReader.cpp InfinibandReader.h Client.cpp Client.h ConfigFileReader.cpp ConfigFileReader.h FakeInfinibandReader.cpp FakeInfinibandReader.h)

include(ExternalProject)

ExternalProject_Add(detector_git
        GIT_REPOSITORY https://github.com/hhu-bsinfo/detector.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/detector
        INSTALL_COMMAND "")

ExternalProject_Get_Property(detector_git binary_dir)
ExternalProject_Get_Property(detector_git source_dir)

add_library(Detector STATIC IMPORTED)
set_property(TARGET Detector PROPERTY IMPORTED_LOCATION ${binary_dir}/ar/libdetector.a)

add_dependencies(Detector detector_git)

target_include_directories(InfiniBandClient PRIVATE ${source_dir}/src)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -I/usr/include/infiniband)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(InfiniBandClient Detector ibverbs ibmad ibnetdisc nlohmann_json::nlohmann_json)