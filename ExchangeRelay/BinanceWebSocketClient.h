#ifndef BINANCEWEBSOCKETCLIENT_H
#define BINANCEWEBSOCKETCLIENT_H

#include <print>

#include "WebSocketClient.h"

class BinanceWebSocketClient
    : public WebSocketClient<BinanceWebSocketClient> {
    friend class WebSocketClient;
public:
    BinanceWebSocketClient() = default;

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
        //client_.send(hdl, subscribe_msg, websocketpp::frame::opcode::text);
    }

    static void OnMessageImpl(const websocketpp::connection_hdl& hdl,
                              const client::message_ptr& pMsg) {
        const auto& msg_str = pMsg->get_payload();
        std::println("Recieved message: {}", msg_str);
    }

    const std::string wss_uri_ = "wss://stream.binance.com:9443/ws/btcusdt@depth";
};

#endif //BINANCEWEBSOCKETCLIENT_H
