#include <llm_todo/commands/command.hpp>
#include <iostream>
#include <cstdlib>

namespace llm_todo {

void Command::report_error(const std::string& message, int exit_code) {
    std::cerr << "Error: " << message << std::endl;
    std::exit(exit_code);
}

} // namespace llm_todo 