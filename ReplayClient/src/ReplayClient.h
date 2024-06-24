#ifndef REPLAYCLIENT_H
#define REPLAYCLIENT_H

#include <string>

class ReplayClient {
public:
    explicit ReplayClient(std::string config_file);

    void run() const;

private:
    const std::string config_file_;
};

#endif  // REPLAYCLIENT_H
