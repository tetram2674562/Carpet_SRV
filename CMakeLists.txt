cmake_minimum_required(VERSION 3.27)
project(CARPET_SRV)

set(CMAKE_CXX_STANDARD 98)

find_package(OpenSSL REQUIRED)

include_directories("include")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(CARPET_SRV ${SOURCES})

target_link_libraries(CARPET_SRV PRIVATE OpenSSL::Crypto)


add_executable(run_tests tests/test_main.cpp)
target_include_directories(run_tests PRIVATE include)