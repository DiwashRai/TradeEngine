
project(ReplayClient)

find_package(CLI11 CONFIG REQUIRED)
find_package(Poco REQUIRED Net NetSSL)

find_library(FIX8_LIBS NAMES fix8 PATHS /usr/local/lib)

add_executable(${PROJECT_NAME} main.cpp
        ../ReplayServer/src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11::CLI11
        Poco::Net
        Poco::NetSSL
        ${FIX8_LIBS})
