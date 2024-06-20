
#include <CLI/CLI.hpp>
#include "ReplayServer.h"

int main(const int argc, char* argv[]) {
    CLI::App cli;
    std::string file_store;
    cli.add_option("-f,--file-store", file_store, "Path to .dbn file store")
        ->required();
    CLI11_PARSE(cli, argc, argv);

    ReplayServer replay_server(file_store);
    replay_server.run();
}
