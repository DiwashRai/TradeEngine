
#include <CLI/CLI.hpp>
#include <fstream>
#include <boost/json.hpp>
#include <jwt-cpp/jwt.h>

#include "CoinbaseWebSocketClient.h"

namespace json = boost::json;

std::pair<std::string, std::string> parseKeyFile(const std::string& key_file) {
    std::ifstream key_file_stream(key_file);
    if (!key_file_stream.is_open()) {
        std::cerr << "Failed to open key file: " << key_file << std::endl;
        return {};
    }

    const std::string json_content((std::istreambuf_iterator<char>(key_file_stream)),
                             std::istreambuf_iterator<char>());
    key_file_stream.close();

    json::error_code ec;
    json::value key_obj = json::parse(json_content, ec);
    if (ec)
        throw std::runtime_error("Failed to parse key file: " + ec.message());

    const std::string key_name = key_obj.at("name").as_string().c_str();
    const std::string key_secret = key_obj.at("privateKey").as_string().c_str();

    return {key_name, key_secret};
}

std::string createJWTToken(const std::string& key_name, const std::string& key_secret) {
    const std::string request_method = "GET";
    const std::string url = "api.coinbase.com";
    const std::string request_path = "/api/v3/brokerage/accounts";
    const std::string uri = request_method + " " + url + request_path;

    unsigned char nonce_raw[16];
    RAND_bytes(nonce_raw, sizeof(nonce_raw));
    std::string nonce(reinterpret_cast<char*>(nonce_raw), sizeof(nonce_raw));
    auto token = jwt::create()
        .set_subject(key_name)
        .set_issuer("cdp")
        .set_not_before(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{120})
        .set_payload_claim("uri", jwt::claim(uri))
        .set_header_claim("kid", jwt::claim(key_name))
        .set_header_claim("nonce", jwt::claim(nonce))
        .sign(jwt::algorithm::es256{key_name, key_secret});
    return token;
}

int main(const int argc, char *argv[]) {
    CLI::App app;

    std::string key_file = "./cdp_api_key.json";
    app.add_option("-k,--key-file", key_file, "Path to key file");
    CLI11_PARSE(app, argc, argv);

    auto [key_name, key_secret] = parseKeyFile(key_file);
    if (key_name.empty() || key_secret.empty())
        return 1;

    const auto token = createJWTToken(key_name, key_secret);
    CoinbaseWebsocketClient client(token);
    client.run();
}
