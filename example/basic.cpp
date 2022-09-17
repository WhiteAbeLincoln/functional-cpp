#include <iostream>
#include <cpparsec/parser.hpp>
#include <cpparsec/combinators.hpp>

namespace cp = cpparsec;

int main() {
    auto V = cp::character('V');
    auto result = cp::parse(V, "Very");
    if (result.index() == 0) {
        std::cout << "Got error" << std::endl;
    }
    else {
        std::cout << "Value: " << std::get<1>(result) << std::endl;
    }
}
