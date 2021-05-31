#include "Algorithms.hpp"

#include <fmt/format.h>

auto main() -> int
{
    fmt::print("{}\n", lab::pow(234, 6565, 4543).str());
    fmt::print("{}\n", lab::is_fermat_prime(3253));
    auto res = lab::extended_euclid(435, 150);
    fmt::print("divisor: {}, x: {}, y: {}\n", res.gcd.str(), res.x.str(), res.y.str());

    fmt::print("is prime: {}\n", lab::miller_rabin(3000));

    auto lhs = cpp_int{"12345634344789123144"};
    auto rhs = cpp_int{"43219876543343442186"};
    fmt::print("{} * {} = {}\n", lhs.str(), rhs.str(), lab::karatsuba(lhs, rhs).str());
}