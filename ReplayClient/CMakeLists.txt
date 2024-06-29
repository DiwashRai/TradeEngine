
project(ReplayClient)

find_package(CLI11 CONFIG REQUIRED)
find_package(Poco REQUIRED Net NetSSL)

add_executable(${PROJECT_NAME} src/main.cpp
        src/ReplayClient.cpp
        src/ReplayClient.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11::CLI11
        Poco::Net
        Poco::NetSSL
        fix8
        Common
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/Common
        ${CMAKE_SOURCE_DIR}/Utils
)
