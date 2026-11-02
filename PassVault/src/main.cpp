// clang-format -i src/*.cpp src/*.hpp

#include <sodium.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "generator.hpp"

namespace {

std::size_t parse_length(const std::string& text) {
    // Confirm string is all digits
    if (text.empty() || text.size() > 4 ||
        text.find_first_not_of("0123456789") != std::string::npos) {
        throw std::invalid_argument("length must be a number between 1 and 1024");
    }
    // Convert string to unsigned long integer
    const unsigned long value = std::stoul(text);
    if (value < 1 || value > 1024) {
        throw std::invalid_argument("length must be a number between 1 and 1024");
    }
    return value;
}

void print_usage() { std::cerr << "usage: passvault generate [--length N] [--no-symbols]\n"; }

int run_generate(const std::vector<std::string>& args) {
    passvault::GeneratorOptions options;

    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (arg == "--length" && i + 1 < args.size()) {
            options.length = parse_length(args[i + 1]);
        } else if (arg == "--no-symbols") {
            options.symbols = false;
        } else {
            std::cerr << "unknown option: " << arg << "\n";
            print_usage();
            return 1;
        }
    }

    std::cout << passvault::generate_password(options) << "\n";
    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (sodium_init() < 0) {
        std::cerr << "error: could not initialize libsodium\n";
        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        print_usage();
        return 1;
    }

    const std::string command = args[0];
    std::vector<std::string> rest(args.begin() + 1, args.end());

    try {
        if (command == "generate") {
            return run_generate(rest);
        }
        std::cerr << "unknown commandL " << command << "\n";
        print_usage();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
