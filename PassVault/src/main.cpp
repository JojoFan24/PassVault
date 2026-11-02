#include <sodium.h>

#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "generator.hpp"

namespace {

constexpr const char* kVersion = "0.2.0";
constexpr std::size_t kMinLength = 1;
constexpr std::size_t kMaxLength = 1024;

using Args = std::vector<std::string>;
using CommandFn = int (*)(const Args&);

struct Command {
    CommandFn run;
    const char* summary;
};

class UsageError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

void print_usage(std::ostream& out);

std::size_t parse_length(const std::string& text) {
    const std::string message = "length must be a number from " + std::to_string(kMinLength) +
                                " to " + std::to_string(kMaxLength);

    if (text.empty() || text.size() > 4 ||
        text.find_first_not_of("0123456789") != std::string::npos) {
        throw UsageError(message);
    }

    const unsigned long value = std::stoul(text);
    if (value < kMinLength || value > kMaxLength) {
        throw UsageError(message);
    }
    return value;
}

const std::string& value_after(const Args& args, std::size_t& i) {
    if (i + 1 >= args.size()) {
        throw UsageError(args[i] + " needs a value");
    }
    ++i;
    return args[i];
}

int run_generate(const Args& args) {
    passvault::GeneratorOptions options;

    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (arg == "--length" || arg == "-l") {
            options.length = parse_length(value_after(args, i));
        } else if (arg == "--no-lowercase") {
            options.lowercase = false;
        } else if (arg == "--no-uppercase") {
            options.uppercase = false;
        } else if (arg == "--no-digits") {
            options.digits = false;
        } else if (arg == "--no-symbols") {
            options.symbols = false;
        } else {
            throw UsageError("unknown option for generate: " + arg);
        }
    }

    std::cout << passvault::generate_password(options) << "\n";
    return 0;
}

int run_version(const Args& args) {
    if (!args.empty()) {
        throw UsageError("version takes no options");
    }
    std::cout << "passvault " << kVersion << "\n";
    return 0;
}

int run_help(const Args& args) {
    if (!args.empty()) {
        throw UsageError("help takes no options");
    }
    print_usage(std::cout);
    return 0;
}

const std::map<std::string, Command> kCommands = {
    {"generate", {run_generate, "Generate a random password"}},
    {"help", {run_help, "Show this help"}},
    {"version", {run_version, "Show the version"}},
};

void print_usage(std::ostream& out) {
    out << "usage: passvault <command> [options]\n\ncommands:\n";
    for (const auto& [name, command] : kCommands) {
        out << "  " << std::left << std::setw(12) << name << command.summary << "\n";
    }
    out << "\ngenerate options:\n"
           "  -l, --length N    password length, 1 to 1024 (default 20)\n"
           "  --no-lowercase    leave out a-z\n"
           "  --no-uppercase    leave out A-Z\n"
           "  --no-digits       leave out 0-9\n"
           "  --no-symbols      leave out symbols\n"
           "\nPasswords always include at least one character of each enabled type.\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (sodium_init() < 0) {
        std::cerr << "error: could not initialize libsodium\n";
        return 1;
    }

    const Args args(argv + 1, argv + argc);
    if (args.empty()) {
        print_usage(std::cerr);
        return 2;
    }

    std::string name = args[0];
    if (name == "-h" || name == "--help") {
        name = "help";
    }
    const Args rest(args.begin() + 1, args.end());

    try {
        const auto it = kCommands.find(name);
        if (it == kCommands.end()) {
            throw UsageError("unknown command: " + name);
        }
        return it->second.run(rest);
    } catch (const UsageError& e) {
        std::cerr << "error: " << e.what() << "\n\n";
        print_usage(std::cerr);
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
