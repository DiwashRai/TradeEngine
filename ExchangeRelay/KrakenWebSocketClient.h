#ifndef KRAKENWEBSOCKETCLIENT_H
#define KRAKENWEBSOCKETCLIENT_H

#include <print>

#include "WebSocketClient.h"

class KrakenWebSocketClient : public WebSocketClient<KrakenWebSocketClient> {
    friend class WebSocketClient;

public:
    KrakenWebSocketClient() = default;

private:
    void OnOpenImpl(const websocketpp::connection_hdl& hdl) {
        std::print("Connection opened\n");

        json::object subscribe_obj;
        subscribe_obj["event"] = "subscribe";
        subscribe_obj["pair"] = json::array{"BTC/USD"};
        json::object subscription;
        subscription["name"] = "book";
        subscribe_obj["subscription"] = subscription;

        const std::string subscribe_msg = json::serialize(subscribe_obj);
        std::println("Subscribe JSON: {}", subscribe_msg);
        client_.send(hdl, subscribe_msg, websocketpp::frame::opcode::text);
    }

    static void OnMessageImpl(const websocketpp::connection_hdl& hdl,
                              const client::message_ptr& pMsg) {
        const auto& msg_str = pMsg->get_payload();

        try {
            if (const auto& v = json::parse(msg_str); v.is_array()) {
                const json::array& arr = v.as_array();
                if (arr.size() <= 1 && !arr[1].is_object()) return;

                const json::object book_data = arr[1].as_object();
                std::println("Received message: {}", msg_str);

            } else if (v.is_object()) {
                std::println("Received message: {}", msg_str);
            }
        } catch (const std::exception& e) {
            std::println(std::cerr, "Error: {}", e.what());
        }
    }

    const std::string wss_uri_ = "wss://ws.kraken.com/";
};

#endif  // KRAKENWEBSOCKETCLIENT_H
