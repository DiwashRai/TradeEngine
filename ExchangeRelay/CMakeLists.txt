
project(ExchangeRelay)

find_package(CLI11 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11::CLI11)
