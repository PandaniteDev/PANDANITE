#include <chrono>
#include <string>
#include <map>

class RateLimiter {
public:
    RateLimiter(size_t N, size_t seconds);
    bool limit(const std::string &s);

private:
    const size_t N;
    const size_t seconds;
    std::map<std::string, std::chrono::steady_clock::time_point> c;

    // Private method to update timestamp for a given key
    void updateTimestamp(const std::string &key);
};

RateLimiter::RateLimiter(size_t N, size_t seconds) : N(N), seconds(seconds) {}

bool RateLimiter::limit(const std::string &s) {
    auto now = std::chrono::steady_clock::now();

    // Update timestamp for the key before checking the limit
    updateTimestamp(s);

    // Count the number of entries within the time window
    size_t count = 0;
    for (const auto &entry : c) {
        if (now - entry.second <= std::chrono::seconds(seconds)) {
            ++count;
        }
    }

    // Check if the limit is exceeded
    return count > N;
}

void RateLimiter::updateTimestamp(const std::string &key) {
    c[key] = std::chrono::steady_clock::now();
} 
