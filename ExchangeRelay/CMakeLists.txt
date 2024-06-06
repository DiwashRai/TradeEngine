
project(ExchangeRelay)

find_package(CLI11 CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(Boost 1.84 COMPONENTS system json REQUIRED)
find_package(OpenSSL REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

add_executable(${PROJECT_NAME} main.cpp
        WebSocketClient.h
        KrakenWebSocketClient.h
        BitfinexWebSocketClient.h
        BinanceWebSocketClient.h
        CoinbaseWebSocketClient.h)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${JWT_CPP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11::CLI11
        websocketpp::websocketpp
        ${Boost_LIBRARIES}
        OpenSSL::SSL)
