
#include <CLI/CLI.hpp>

#include "ReplayServer.h"

int main(const int argc, char* argv[]) {
    CLI::App cli;
    std::string data_file;
    cli.add_option("-d,--data-file", data_file, "Path to .dbn data file")
        ->required();

    std::string config_file;
    cli.add_option("-c,--config-file", config_file, "Path to config file")
        ->required();
    CLI11_PARSE(cli, argc, argv);

    ReplayServer replay_server(data_file, config_file);
    replay_server.run();
}
