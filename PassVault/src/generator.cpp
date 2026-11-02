// Password Generator

#include "generator.hpp"

#include <sodium.h>

#include <cstdint>
#include <stdexcept>

namespace passvault {

std::string generate_password(const GeneratorOptions& options) {
    if (options.length == 0) {
        throw std::invalid_argument("Length must be at least 1");
    }

    std::string alphabet;
    if (options.lowercase) alphabet += "abcdefghijklmnopqrstuvwxyz";
    if (options.uppercase) alphabet += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (options.digits) alphabet += "0123456789";
    if (options.symbols) alphabet += "!@#$^&*()-_=+[]{};;,.?";

    if (alphabet.empty()) {
        throw std::invalid_argument("at least one character set must be enabled");
    }

    std::string password;
    password.reserve(options.length);

    // Randomly generate a password (TRUE RANDOM)
    for (std::size_t i = 0; i < options.length; ++i) {
        // Lib sodium requires 32 bit
        const auto upper = static_cast<uint32_t>(alphabet.size());
        const uint32_t index = randombytes_uniform(upper);
        password += alphabet[index];
    }

    return password;
}
}  // namespace passvault
