cmake_minimum_required(VERSION 3.20)
project(llm_todo VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Include directories
include_directories(include)

# Third-party dependencies (header-only libraries)
add_subdirectory(third_party)

# Main executable
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(llm_todo ${SOURCES})

# Link libraries
target_link_libraries(llm_todo PRIVATE nlohmann_json cxxopts)

# Enable testing
enable_testing()

# Test executable
file(GLOB_RECURSE TEST_SOURCES 
    "tests/*.cpp"
    "src/task.cpp"
    "src/store.cpp"
    "src/commands/*.cpp"
)

add_executable(llm_todo_tests ${TEST_SOURCES})
target_link_libraries(llm_todo_tests PRIVATE nlohmann_json cxxopts Catch2::Catch2WithMain)

# Register tests
add_test(NAME unit_tests COMMAND llm_todo_tests)

# Coverage support (optional)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(llm_todo_tests PRIVATE --coverage)
        target_link_options(llm_todo_tests PRIVATE --coverage)
    endif()
endif()

# clang-tidy target (optional)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_EXE} 
        ${SOURCES}
        -p ${CMAKE_BINARY_DIR}
        --
        -std=c++20
        -I${CMAKE_SOURCE_DIR}/include
    )
endif()

# Installation
install(TARGETS llm_todo
    RUNTIME DESTINATION bin
) 