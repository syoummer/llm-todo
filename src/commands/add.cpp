#include <llm_todo/commands/add.hpp>
#include <llm_todo/store.hpp>

namespace llm_todo {

int AddCommand::execute(Store& store, const cxxopts::ParseResult& args) {
    (void)store; (void)args; // Suppress unused parameter warnings
    // TODO: Implement add command
    return 0;
}

void AddCommand::add_options(cxxopts::Options& options) {
    (void)options; // Suppress unused parameter warning
    // TODO: Add command-specific options
}

std::string AddCommand::get_description() const {
    return "Add a new task";
}

std::string AddCommand::get_name() const {
    return "add";
}

bool AddCommand::validate_description(const std::string& description) const {
    return description.size() <= 10 * 1024;
}

bool AddCommand::validate_tags(const std::vector<std::string>& tags) const {
    (void)tags; // Suppress unused parameter warning
    // TODO: Implement tag validation
    return true;
}

} // namespace llm_todo 