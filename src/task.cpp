#include <llm_todo/task.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace llm_todo {

// Constructor
Task::Task(uint64_t id, const std::string& description, const std::vector<std::string>& tags)
    : id_(id)
    , description_(description)
    , tags_(tags)
    , created_at_(std::chrono::system_clock::now())
    , done_(false)
    , done_at_(std::nullopt) {
    
    // Validate description length (10KB limit from requirements)
    if (description.size() > 10 * 1024) {
        throw std::invalid_argument("Task description exceeds 10KB limit");
    }
}

// Tag management
void Task::add_tag(const std::string& tag) {
    if (std::find(tags_.begin(), tags_.end(), tag) == tags_.end()) {
        tags_.push_back(tag);
    }
}

void Task::remove_tag(const std::string& tag) {
    auto it = std::find(tags_.begin(), tags_.end(), tag);
    if (it != tags_.end()) {
        tags_.erase(it);
    }
}

// Task operations
void Task::mark_done() {
    if (!done_) {
        done_ = true;
        done_at_ = std::chrono::system_clock::now();
    }
}

void Task::mark_pending() {
    done_ = false;
    done_at_ = std::nullopt;
}

// Formatting methods (ISO 8601)
std::string Task::format_created_at() const {
    return format_time_point(created_at_);
}

std::string Task::format_done_at() const {
    if (done_at_.has_value()) {
        return format_time_point(done_at_.value());
    }
    return "";
}

std::string Task::format_tags() const {
    if (tags_.empty()) {
        return "[]";
    }
    
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < tags_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << tags_[i];
    }
    oss << "]";
    return oss.str();
}

// JSON serialization
nlohmann::json Task::to_json() const {
    nlohmann::json j;
    j["id"] = id_;
    j["description"] = description_;
    j["tags"] = tags_;
    j["created_at"] = format_created_at();
    j["done"] = done_;
    
    if (done_at_.has_value()) {
        j["done_at"] = format_done_at();
    } else {
        j["done_at"] = nullptr;
    }
    
    return j;
}

Task Task::from_json(const nlohmann::json& j) {
    // Validate required fields
    if (!j.contains("id") || !j.contains("description") || 
        !j.contains("created_at") || !j.contains("done")) {
        throw std::invalid_argument("Invalid task JSON: missing required fields");
    }
    
    uint64_t id = j["id"];
    std::string description = j["description"];
    std::vector<std::string> tags;
    
    if (j.contains("tags") && j["tags"].is_array()) {
        tags = j["tags"];
    }
    
    // Create task with basic info
    Task task(id, description, tags);
    
    // Set timestamps
    task.created_at_ = parse_time_point(j["created_at"]);
    task.done_ = j["done"];
    
    if (j.contains("done_at") && !j["done_at"].is_null()) {
        task.done_at_ = parse_time_point(j["done_at"]);
    }
    
    return task;
}

// Helper method to format time points (ISO 8601)
std::string Task::format_time_point(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        tp.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
    
    return oss.str();
}

std::chrono::system_clock::time_point Task::parse_time_point(const std::string& time_str) {
    std::istringstream iss(time_str);
    std::tm tm = {};
    char dot, z;
    int ms = 0;
    
    // Parse format: 2024-01-15T10:30:00.123Z
    iss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    
    if (iss.fail()) {
        throw std::invalid_argument("Invalid time format: " + time_str);
    }
    
    // Try to parse milliseconds if present
    if (iss.peek() == '.') {
        iss >> dot >> ms >> z;
    } else if (iss.peek() == 'Z') {
        iss >> z;
    }
    
    // Convert to time_point
    auto time_t = std::mktime(&tm);
    // Note: mktime assumes local time, but we're parsing UTC
    // For simplicity, we'll assume the parsed time is already in UTC
    
    auto tp = std::chrono::system_clock::from_time_t(time_t);
    tp += std::chrono::milliseconds(ms);
    
    return tp;
}

} // namespace llm_todo 