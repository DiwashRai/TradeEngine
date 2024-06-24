
#include <CLI/CLI.hpp>

#include "ReplayClient.h"

int main(const int argc, char* argv[]) {
    CLI::App cli;
    std::string config_file;
    cli.add_option("-c,--config", config_file, "Configuration file path.")
        ->required();
    CLI11_PARSE(cli, argc, argv);

    ReplayClient replay_client(config_file);
    replay_client.run();
}
