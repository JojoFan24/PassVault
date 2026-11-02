#include "generator.hpp"

#include <sodium.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace passvault {

namespace {

constexpr std::string_view kLowercase = "abcdefghijklmnopqrstuvwxyz";
constexpr std::string_view kUppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view kDigits = "0123456789";
constexpr std::string_view kSymbols = "!@#$%^&*()-_=+[]{};:,.?";

std::string random_string(std::string_view alphabet, std::size_t length) {
    const auto upper = static_cast<uint32_t>(alphabet.size());

    std::string result;
    result.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        result += alphabet[randombytes_uniform(upper)];
    }
    return result;
}

bool contains_every_set(const std::string& candidate,
                        const std::vector<std::string_view>& required_sets) {
    for (const std::string_view set : required_sets) {
        if (candidate.find_first_of(set) == std::string::npos) {
            return false;
        }
    }
    return true;
}

}  // namespace

std::string generate_password(const GeneratorOptions& options) {
    std::vector<std::string_view> required_sets;
    if (options.lowercase) required_sets.push_back(kLowercase);
    if (options.uppercase) required_sets.push_back(kUppercase);
    if (options.digits)    required_sets.push_back(kDigits);
    if (options.symbols)   required_sets.push_back(kSymbols);

    if (required_sets.empty()) {
        throw std::invalid_argument("at least one character set must be enabled");
    }
    if (options.length < required_sets.size()) {
        throw std::invalid_argument("length must be at least " +
                                    std::to_string(required_sets.size()) +
                                    " to include every enabled character type");
    }

    std::string alphabet;
    for (const std::string_view set : required_sets) {
        alphabet += set;
    }

    while (true) {
        std::string candidate = random_string(alphabet, options.length);
        if (contains_every_set(candidate, required_sets)) {
            return candidate;
        }
        sodium_memzero(candidate.data(), candidate.size());
    }
}

}  // namespace passvault
