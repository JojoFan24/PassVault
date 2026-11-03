
#pragma once

#include <cstddef>
#include <string>

#include "secure_buffer.hpp"

namespace passvault {

struct GeneratorOptions {
    std::size_t length = 20;
    bool lowercase = true;
    bool uppercase = true;
    bool digits = true;
    bool symbols = true;
};

SecureBuffer generate_password(const GeneratorOptions& options);

}  // namespace passvault
