cmake_minimum_required(VERSION 3.28)
project(TradeEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS off)

set(CMAKE_CXX_COMPILER "$ENV{HOME}/source_builds/gcc-14-build/bin/g++")
set(Boost_NO_WARN_NEW_VERSIONS 1)

if (UNIX AND NOT APPLE)
    link_directories("$ENV{HOME}/source_builds/gcc-14-build/lib64")
endif()

add_subdirectory(Common)
add_subdirectory(ReplayClient)
add_subdirectory(ReplayServer)
add_subdirectory(Utils)
