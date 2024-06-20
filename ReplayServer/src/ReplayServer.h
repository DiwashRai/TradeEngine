#ifndef REPLAYSERVER_H
#define REPLAYSERVER_H

#include <databento/dbn_file_store.hpp>
#include <string>
#include <print>

using namespace databento;

class ReplayServer {
public:
    explicit ReplayServer(const std::string& file_path)
        : file_path_(file_path) {}

    void run() {
        DbnFileStore store(file_path_);

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
    }

private:
    const std::string file_path_;
};

#endif  // REPLAYSERVER_H
