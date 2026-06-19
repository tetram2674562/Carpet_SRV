cmake_minimum_required(VERSION 3.27)
project(CARPET_SRV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CPM.cmake)

set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party)

CPMAddPackage(
        NAME TBB
        GITHUB_REPOSITORY oneapi-src/oneTBB
        VERSION 2022.3.0
)

FetchContent_Declare(
        openssl-cmake
        URL https://github.com/jimmy-park/openssl-cmake/archive/main.tar.gz
)
FetchContent_MakeAvailable(openssl-cmake)

CPMAddPackage(
        NAME zlib
        GITHUB_REPOSITORY madler/zlib
        VERSION 1.3.1
)

CPMAddPackage(
        NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        GIT_TAG asio-1-36-0
        DOWNLOAD_ONLY YES
)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE
        ${asio_SOURCE_DIR}/asio/include
)
include_directories("include")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(CARPET_SRV ${SOURCES})

target_link_libraries(CARPET_SRV PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
target_link_libraries(CARPET_SRV PRIVATE OpenSSL::Crypto)
target_link_libraries(CARPET_SRV PRIVATE zlib)


add_executable(run_tests tests/test_main.cpp)
target_include_directories(run_tests PRIVATE include)