#ifndef BITFINEXWEBSOCKETCLIENT_H
#define BITFINEXWEBSOCKETCLIENT_H

#include <print>

#include "WebSocketClient.h"

class BitfinexWebSocketClient
    : public WebSocketClient<BitfinexWebSocketClient> {
    friend class WebSocketClient;

public:
    BitfinexWebSocketClient() = default;

private:
    void OnOpenImpl(const websocketpp::connection_hdl& hdl) {
        std::print("Connection opened\n");

        json::object subscribe_obj;
        subscribe_obj["event"] = "subscribe";
        subscribe_obj["channel"] = "book";
        subscribe_obj["symbol"] = "tBTCUSD";
        subscribe_obj["prec"] = "R0";

        const std::string subscribe_msg = json::serialize(subscribe_obj);
        client_.send(hdl, subscribe_msg, websocketpp::frame::opcode::text);
    }

    static void OnMessageImpl(const websocketpp::connection_hdl& hdl,
                              const client::message_ptr& pMsg) {
        const auto& msg_str = pMsg->get_payload();

        try {
            if (const auto& v = json::parse(msg_str); v.is_array()) {
                const json::array& arr = v.as_array();
                if (arr.size() <= 1 && !arr[1].is_array()) return;

                const json::array book_data = arr[1].as_array();
                if (book_data.size() <= 1) return;

                if (book_data[1].is_array())
                    OnSnapShot(book_data, msg_str);
                else
                    OnUpdate(book_data, msg_str);
            } else if (v.is_object()) {  // usually a response json
                std::println("Received message: {}", msg_str);
            }
        } catch (const std::exception& e) {
            std::println(std::cerr, "Error: {}", e.what());
        }
    }

    static void OnSnapShot(const json::array& book_data,
                           const std::string& msg_str) {
        std::println("Snapshot: {}", msg_str);
    }

    static void OnUpdate(const json::array& book_data,
                         const std::string& msg_str) {
        std::println("Update: {}", msg_str);
    }

    const std::string wss_uri_ = "wss://api-pub.bitfinex.com/ws/2";
};

#endif  // BITFINEXWEBSOCKETCLIENT_H
