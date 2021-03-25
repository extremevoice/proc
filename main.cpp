#include <iostream>
#include <string>

#include "src/headers/lexer.h"

#define LOG(n) std::cout << n << std::endl;

int main() {
    std::string_view source { "n .= @data {10 int}" };

    try {
        auto tokens = lexer::Lexer(source).Tokenize();
        for (const auto& token : tokens) {
            LOG(token);
        }
    } catch (std::exception& exception) {
        std::cerr << "Exception: " << exception.what() << std::endl;
        std::cerr << "Terminating compiler" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}
