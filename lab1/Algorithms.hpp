#pragma once

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

namespace lab {

    auto is_fermat_prime(const cpp_int& x, int iters = 5) -> bool;

    auto miller_rabin(const cpp_int& n, int iters = 5) -> bool;

    auto pow(const cpp_int& x, const cpp_int& degree, const cpp_int& mod) -> cpp_int;

    struct EuclidResult {
        cpp_int gcd;
        cpp_int x;
        cpp_int y;
    };
    /// ax + by = gcd(a, b)
    auto extended_euclid(const cpp_int& a, const cpp_int& b) -> EuclidResult;

    /// Gavno, much worse than default cpp_int multiplication
    auto karatsuba(const cpp_int& lhs, const cpp_int& rhs) -> cpp_int;
}