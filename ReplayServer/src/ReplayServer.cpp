
#include "ReplayServer.h"

#include <print>
#include <fix8/f8includes.hpp>

#include "SignalHandler.h"
#include "db_classes.hpp"
#include "db_session.hpp"

using namespace databento;
using namespace FIX8;

ReplayServer::ReplayServer(std::string data_file, std::string config_file)
        : data_file_(std::move(data_file)),
          config_file_(std::move(config_file)) {
    GlobalLogger::set_global_filename("./log/ReplayServer.log");
}

void ReplayServer::run() const {
    ServerSession<db_session_server> server(FIX4200_ctx(), config_file_,
                                            "ReplayServer");
    SignalHandler::initialise();

    while (!SignalHandler::termination_requested()) {
        if (!server.poll()) continue;

        std::print("Inbound connection detected. Starting instance...\n");
        SessionInstance<db_session_server> inst(server);
        inst.start(true);
        inst.stop();
    }

    /*
    DbnFileStore store(data_file_);

    std::vector<MboMsg> msgs;
    auto print_large_trades = [&msgs](const Record record) {
        const auto& mbo_msg = record.Get<MboMsg>();
        msgs.emplace_back(mbo_msg);
        return KeepGoing::Continue;
    };

    const auto start = std::chrono::high_resolution_clock::now();

    store.Replay(print_large_trades);

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::microseconds duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::println("{} microseconds", duration.count());
    */
}
