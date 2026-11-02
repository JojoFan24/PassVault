
#pragma once

#include <cstddef>
#include <string>

namespace passvault {

struct GeneratorOptions {
  std::size_t length = 20;
  bool lowercase = true;
  bool uppercase = true;
  bool digits = true;
  bool symbols = true;
};

std : string generate_password(const GeneratorOptions &options);

} // namespace passvault
