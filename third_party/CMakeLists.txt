# Third-party dependencies for llm_todo

# nlohmann/json for JSON parsing
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE nlohmann/single_include)

# cxxopts for command-line parsing
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE cxxopts/include)

# Catch2 for unit testing
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE catch2/single_include)

# Catch2 with main
add_library(Catch2WithMain ${CMAKE_CURRENT_SOURCE_DIR}/catch2/single_include/catch2/catch_amalgamated.cpp)
target_include_directories(Catch2WithMain PUBLIC catch2/single_include)
add_library(Catch2::Catch2WithMain ALIAS Catch2WithMain) 