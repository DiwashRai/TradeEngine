#ifndef COINBASEWEBSOCKETCLIENT_H
#define COINBASEWEBSOCKETCLIENT_H

#include <curlpp/cURLpp.hpp>
#include <print>
#include <utility>

#include "WebSocketClient.h"

class CoinbaseWebsocketClient
    : public WebSocketClient<CoinbaseWebsocketClient> {
    friend class WebSocketClient;

public:
    explicit CoinbaseWebsocketClient(std::string token)
        : token_(std::move(token))
    {}

private:
    void OnOpenImpl(const websocketpp::connection_hdl& hdl) {
        std::print("Connection opened\n");

        json::object subscribe_obj;
        subscribe_obj["type"] = "subscribe";
        subscribe_obj["product_ids"] = json::array{"BTC-USD"};
        subscribe_obj["channel"] = "full";
        // subscribe_obj["key"] =
        //     "organizations/14dcd262-bab6-47be-a8bb-336397d45e50/apiKeys/"
        //     "23309300-b825-4290-8742-32f6f49f4c00";
        //subscribe_obj["jwt"] = token_;

        const std::string subscribe_msg = json::serialize(subscribe_obj);
        std::println("Subscribe JSON: {}", subscribe_msg);
        client_.send(hdl, subscribe_msg, websocketpp::frame::opcode::text);
    }

    static void OnMessageImpl(const websocketpp::connection_hdl& hdl,
                              const client::message_ptr& pMsg) {
        const auto& msg_str = pMsg->get_payload();
        std::println("Recieved message: {}", msg_str);
    }

    //const std::string wss_uri_ = "wss://ws-feed.exchange.coinbase.com";
    const std::string wss_uri_ = "wss://advanced-trade-ws.coinbase.com";
    const std::string token_;
};

#endif  // COINBASEWEBSOCKETCLIENT_H
