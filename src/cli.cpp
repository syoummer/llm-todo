#include <llm_todo/cli.hpp>
#include <iostream>

namespace llm_todo {

// Constructor
CLI::CLI(const std::string& store_path)
    : store_(std::make_unique<Store>(store_path.empty() ? "~/.llm_todo.json" : store_path)) {
    setup_commands();
}

// Main entry point
int CLI::run(int argc, char* argv[]) {
    (void)argc; (void)argv; // Suppress unused parameter warnings
    // TODO: Implement full CLI logic
    std::cout << "LLM Todo v1.0.0 - Command line interface not yet implemented" << std::endl;
    return 0;
}

// Private methods
void CLI::setup_commands() {
    // TODO: Setup commands
}

void CLI::setup_options(cxxopts::Options& options) {
    (void)options; // Suppress unused parameter warning
    // TODO: Setup options
}

int CLI::execute_command(const cxxopts::ParseResult& result) {
    (void)result; // Suppress unused parameter warning
    // TODO: Implement command execution
    return 0;
}

void CLI::print_help(const cxxopts::Options& options) {
    (void)options; // Suppress unused parameter warning
    // TODO: Implement help printing
}

void CLI::print_version() {
    // TODO: Implement version printing
}

void CLI::report_error(const std::string& message, int exit_code) {
    std::cerr << "Error: " << message << std::endl;
    std::exit(exit_code);
}

} // namespace llm_todo 