
#include <CLI/CLI.hpp>
#include <print>

int main(const int argc, char *argv[]) {
    CLI::App app;
    CLI11_PARSE(app, argc, argv);
    std::println("Hello world!");
}
