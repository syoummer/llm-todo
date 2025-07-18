# LLM Todo List - Implementation Plan & Execution Guide

## Phase 1: Project Structure & Foundation (Day 1)

### 1.1 Repository Setup
```
llm_todo/
├── CMakeLists.txt
├── include/
│   └── llm_todo/
│       ├── task.hpp
│       ├── store.hpp
│       ├── cli.hpp
│       └── commands/
│           ├── add.hpp
│           ├── list.hpp
│           ├── done.hpp
│           └── rm.hpp
├── src/
│   ├── main.cpp
│   ├── task.cpp
│   ├── store.cpp
│   ├── cli.cpp
│   └── commands/
│       ├── add.cpp
│       ├── list.cpp
│       ├── done.cpp
│       └── rm.cpp
├── tests/
│   ├── test_main.cpp
│   ├── test_task.cpp
│   ├── test_store.cpp
│   └── test_commands.cpp
├── third_party/
│   ├── nlohmann/
│   ├── cxxopts/
│   └── catch2/
├── docker/
│   └── Dockerfile
└── README.md
```

### 1.2 CMake Configuration
- **Root CMakeLists.txt**: Configure C++20, compiler flags, dependencies
- **Target definition**: Create `llm_todo` executable
- **Test configuration**: Integrate Catch2, enable coverage
- **Third-party integration**: Header-only libraries as subdirectories
- **clang-tidy target**: Optional static analysis

### 1.3 Core Data Structures
**Task class** (`include/llm_todo/task.hpp`):
```cpp
class Task {
private:
    uint64_t id_;
    std::string description_;
    std::vector<std::string> tags_;
    std::chrono::system_clock::time_point created_at_;
    bool done_;
    std::optional<std::chrono::system_clock::time_point> done_at_;

public:
    // Constructors, getters, setters
    std::string format_created_at() const;  // ISO 8601: "2024-01-15T10:30:00Z"
    std::string format_done_at() const;     // ISO 8601 or empty if not done
    std::string format_tags() const;        // "[work, urgent]" or "[]"
    nlohmann::json to_json() const;
    static Task from_json(const nlohmann::json& j);
};
```

## Phase 2: Storage Layer (Day 2)

### 2.1 JSON Store Implementation
**Store class** (`include/llm_todo/store.hpp`):
```cpp
class Store {
private:
    std::filesystem::path store_path_;
    uint64_t next_id_;
    std::vector<Task> tasks_;

public:
    void load();  // Lazy loading
    void save();  // Atomic write (temp file + rename)
    uint64_t add_task(const std::string& desc, const std::vector<std::string>& tags);
    bool mark_done(uint64_t id);
    bool remove_task(uint64_t id);
    std::vector<Task> get_all_tasks() const;
    std::vector<Task> get_pending_tasks() const;
    std::vector<Task> get_done_tasks() const;
};
```

### 2.2 JSON Schema Implementation
```json
{
  "version": "1.0",
  "next_id": 1,
  "tasks": [
    {
      "id": 1,
      "description": "Buy groceries",
      "tags": ["personal", "urgent"],
      "created_at": "2024-01-15T10:30:00Z",
      "done": false,
      "done_at": null
    }
  ]
}
```

### 2.3 File Operations
- **Atomic writes**: Write to temp file, then rename
- **Path sanitization**: Use `std::filesystem::path` for security
- **Error handling**: Graceful handling of permission errors, disk full, etc.

## Phase 3: CLI Framework (Day 3)

### 3.1 Command Line Parsing
**CLI class** (`include/llm_todo/cli.hpp`):
```cpp
class CLI {
public:
    int run(int argc, char* argv[]);
private:
    void setup_options();
    int execute_command(const cxxopts::ParseResult& result);
};
```

### 3.2 Command Structure
Each command implements:
```cpp
class Command {
public:
    virtual ~Command() = default;
    virtual int execute(Store& store, const cxxopts::ParseResult& args) = 0;
    virtual void add_options(cxxopts::Options& options) = 0;
};
```

### 3.3 Exit Code Strategy
- **0**: Success
- **1**: User errors (invalid ID, task not found, invalid arguments, malformed command syntax, description too long)
- **2**: System errors (file I/O failures, JSON parsing errors, memory allocation failures, filesystem permission errors, disk full)

## Phase 4: Core Commands Implementation (Day 4-5)

### 4.1 Add Command
```bash
llm_todo add "Buy groceries" --tag personal --tag urgent
# Output: Task 1 added
```
- **Input format**: Multiple `--tag` flags for multiple tags
- **Validation**: Description length (<10KB), tag format validation
- **Output**: Simple confirmation with new task ID
- **Timestamps**: Auto-generate `created_at` in ISO 8601 format

### 4.2 List Command
```bash
llm_todo list [--all | --pending | --done]
# Default behavior: --pending
```
Output format:
```
ID  Description              Tags           Created
1   Buy groceries           [personal, urgent]  2024-01-15 10:30
2   ✓ Read documentation    [work]             2024-01-14 09:15
```

### 4.3 Done Command
```bash
llm_todo done 1
# Output: Task 1 marked as done
```
- Idempotent operation
- Set `done_at` timestamp

### 4.4 Remove Command
```bash
llm_todo rm 1
# Output: Task 1 removed
```
- Permanent deletion
- Validate ID exists

## Phase 5: Testing Framework (Day 6)

### 5.1 Unit Test Structure
```cpp
// test_task.cpp
TEST_CASE("Task creation and JSON serialization") { ... }
TEST_CASE("Task completion") { ... }

// test_store.cpp  
TEST_CASE("Store initialization") { ... }
TEST_CASE("Task persistence") { ... }
TEST_CASE("Atomic write operations") { ... }

// test_commands.cpp
TEST_CASE("Add command with tags") { ... }
TEST_CASE("List command filtering") { ... }
TEST_CASE("Done command idempotency") { ... }
```

### 5.2 Coverage Targets
- **Core logic**: ≥90% line coverage
- **Error paths**: Test all exit code scenarios
- **Edge cases**: Empty store, corrupted JSON, large descriptions

### 5.3 Test Utilities
```cpp
class TestStore {
    // In-memory store for testing
    // Temporary file handling
};
```

## Phase 6: Error Handling & Security (Day 7)

### 6.1 Input Validation
- Task description size limit (10KB)
- Path traversal prevention
- JSON structure validation

### 6.2 Error Reporting
```cpp
enum class ErrorType {
    USER_ERROR = 1,
    SYSTEM_ERROR = 2
};

void report_error(const std::string& msg, ErrorType type);
```

### 6.3 Logging Strategy
- Human-readable error messages to stderr
- UTF-8 support throughout

## Phase 7: Build System & CI (Day 8)

### 7.1 CMake Features
- Cross-platform configuration
- RelWithDebInfo default build type
- Optional clang-tidy integration
- CTest integration

### 7.2 GitHub Actions
```yaml
strategy:
  matrix:
    os: [ubuntu-latest, macos-latest]
    compiler: [clang, gcc]
```

### 7.3 Performance Testing
- Benchmark with 10,000 tasks
- Target: `list` command < 50ms

## Phase 8: Containerization (Day 9)

### 8.1 Multi-stage Dockerfile
```dockerfile
# Build stage
FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y clang cmake git
COPY . /src
WORKDIR /src
RUN cmake -B build && cmake --build build

# Runtime stage  
FROM ubuntu:24.04
RUN useradd -m todouser
COPY --from=builder /src/build/llm_todo /usr/local/bin/
USER todouser
ENTRYPOINT ["llm_todo"]
```

### 8.2 Image Optimization
- Target: <50MB compressed
- Minimal runtime dependencies
- Non-root user execution

## Phase 9: Stretch Goals (Day 10+)

### 9.1 Edit Command
```bash
llm_todo edit 1 --desc "New description" --tag newtag
```

### 9.2 Search Command
```bash
llm_todo search "grocery" [--regex]
```

### 9.3 Export Command
```bash
llm_todo export --output tasks.csv --format csv
```

## Dependencies & Integration

### Required Libraries
- **nlohmann/json**: JSON parsing/serialization
- **cxxopts**: Command-line argument parsing  
- **Catch2**: Unit testing framework

### Development Tools
- **CMake ≥3.20**: Build system
- **clang-tidy**: Static analysis
- **gcov/llvm-cov**: Coverage reporting

## Risk Mitigation

### Technical Risks
- **JSON corruption**: Implement validation and backup
- **Performance**: Profile early with large datasets
- **Cross-platform**: Test on both Linux and macOS

### Dependency Risks
- **Header-only constraint**: Ensure all libs are header-only
- **Version compatibility**: Pin to specific versions

## Acceptance Testing

### Manual Test Cases
1. Add task with multiple tags
2. List tasks in different states
3. Complete and remove tasks
4. Handle corrupted JSON file
5. Test with 10,000 tasks

### Automated Validation
- All unit tests pass
- clang-tidy zero critical warnings
- Docker image builds and runs
- Coverage ≥90% on core logic

## Implementation Decisions Summary

### Clarified Requirements
- **List default behavior**: `--pending` (show only incomplete tasks)
- **Tag input format**: Multiple `--tag` flags (`--tag work --tag urgent`)
- **Tag output format**: Comma-separated in brackets (`[work, urgent]`)
- **Timestamp format**: ISO 8601 (`2024-01-15T10:30:00Z`)

### Error Code Classification
- **Exit Code 1**: Invalid ID, task not found, invalid arguments, malformed command syntax, description too long
- **Exit Code 2**: File I/O failures, JSON parsing errors, memory allocation failures, filesystem permission errors, disk full

### JSON Schema Design
- Root object contains `version`, `next_id`, and `tasks` array
- Each task has `id`, `description`, `tags`, `created_at`, `done`, `done_at`
- Timestamps stored as ISO 8601 strings
- Tags stored as string arrays

---

**Total Estimated Timeline: 10-12 days**

This plan provides concrete, executable steps for implementing the LLM Todo application while maintaining flexibility for LLM-assisted development and iteration. 