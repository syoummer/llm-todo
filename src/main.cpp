#include <iostream>
#include <exception>
#include <llm_todo/cli.hpp>

int main(int argc, char* argv[]) {
    try {
        llm_todo::CLI cli;
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2; // System error
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 2; // System error
    }
} 