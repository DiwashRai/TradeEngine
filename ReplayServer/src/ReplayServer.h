#ifndef REPLAYSERVER_H
#define REPLAYSERVER_H

#include <databento/dbn_file_store.hpp>
#include <string>

class ReplayServer {
public:
    explicit ReplayServer(std::string data_file, std::string config_file);

    void run() const;

private:
    const std::string data_file_;
    const std::string config_file_;
};

#endif  // REPLAYSERVER_H
