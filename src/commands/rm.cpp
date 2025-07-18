#include <llm_todo/commands/rm.hpp>
#include <llm_todo/store.hpp>

namespace llm_todo {

int RemoveCommand::execute(Store& store, const cxxopts::ParseResult& args) {
    (void)store; (void)args; // Suppress unused parameter warnings
    // TODO: Implement remove command
    return 0;
}

void RemoveCommand::add_options(cxxopts::Options& options) {
    (void)options; // Suppress unused parameter warning
    // TODO: Add command-specific options
}

std::string RemoveCommand::get_description() const {
    return "Remove task permanently";
}

std::string RemoveCommand::get_name() const {
    return "rm";
}

bool RemoveCommand::validate_task_id(const std::string& id_str, uint64_t& id) const {
    (void)id_str; (void)id; // Suppress unused parameter warnings
    // TODO: Implement ID validation
    return true;
}

} // namespace llm_todo 