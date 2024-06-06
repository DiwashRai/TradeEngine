#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <boost/json.hpp>
#include <print>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

namespace json = boost::json;

template <typename Client>
class WebSocketClient {
public:
    using client = websocketpp::client<websocketpp::config::asio_tls_client>;
    using context_ptr = websocketpp::lib::shared_ptr<boost::asio::ssl::context>;

    WebSocketClient() {
        client_.set_access_channels(websocketpp::log::alevel::all);
        client_.clear_access_channels(websocketpp::log::alevel::frame_payload);
        client_.set_error_channels(websocketpp::log::elevel::all);

        client_.init_asio();
        client_.set_tls_init_handler(&WebSocketClient::OnTlsInit);

        client_.set_open_handler(
            [this](websocketpp::connection_hdl hdl) { this->OnOpen(hdl); });

        client_.set_message_handler(
            [this](websocketpp::connection_hdl hdl, client::message_ptr msg) {
                this->OnMessage(hdl, msg);
            });

        client_.set_close_handler(
            [this](websocketpp::connection_hdl hdl) { this->OnClose(hdl); });

        client_.set_fail_handler(
            [this](websocketpp::connection_hdl hdl) { this->OnFail(hdl); });
    }

    void run() {
        try {
            websocketpp::lib::error_code ec;
            client::connection_ptr con = client_.get_connection(
                static_cast<Client*>(this)->wss_uri_, ec);
            if (ec) {
                std::println("could not create connection because: {}",
                             ec.message());
                return;
            }
            client_.connect(con);
            client_.run();
        } catch (const std::exception& e) {
            std::println(std::cerr, "Error: {}", e.what());
        }
    }

protected:
    static context_ptr OnTlsInit(const websocketpp::connection_hdl&) {
        context_ptr ctx =
            websocketpp::lib::make_shared<boost::asio::ssl::context>(
                boost::asio::ssl::context::tlsv12);
        try {
            ctx->set_options(boost::asio::ssl::context::default_workarounds |
                             boost::asio::ssl::context::no_sslv2 |
                             boost::asio::ssl::context::no_sslv3 |
                             boost::asio::ssl::context::single_dh_use);
        } catch (std::exception& e) {
            std::println("Error in context pointer: {}", e.what());
        }
        return ctx;
    }
    void OnOpen(websocketpp::connection_hdl hdl) {
        static_cast<Client*>(this)->OnOpenImpl(hdl);
    }

    void OnMessage(websocketpp::connection_hdl hdl, client::message_ptr pMsg) {
        static_cast<Client*>(this)->OnMessageImpl(hdl, pMsg);
    }

    void OnClose(const websocketpp::connection_hdl&) {
        std::println("Connection closed");
    }

    void OnFail(const websocketpp::connection_hdl&) {
        std::println("Connection failed");
    }

    client client_;
};

#endif  // WEBSOCKET_H
