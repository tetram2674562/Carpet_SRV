cmake_minimum_required(VERSION 3.27)
project(CARPET_SRV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(TBB CONFIG REQUIRED)
include_directories("include")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(CARPET_SRV ${SOURCES})

target_link_libraries(CARPET_SRV PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
target_link_libraries(CARPET_SRV PRIVATE OpenSSL::Crypto)
target_link_libraries(CARPET_SRV PRIVATE ZLIB::ZLIB)


add_executable(run_tests tests/test_main.cpp)
target_include_directories(run_tests PRIVATE include)