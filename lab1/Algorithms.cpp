#include "Algorithms.hpp"

#include <boost/random.hpp>
#include <boost/multiprecision/number.hpp>

namespace {
    auto random(const auto& min, const auto& max) {
        static boost::mt19937 mt{};
        boost::random::uniform_int_distribution<std::decay_t<decltype(max)>> dist(min, max);
        return dist(mt);
    }
}

auto lab::pow(const cpp_int& x, const cpp_int& degree, const cpp_int& mod) -> cpp_int
{
    if (x == 0) {
        return 0;
    }
    if (degree == 0) {
        return 1;
    }
    const auto temp = pow(x, degree / 2, mod);
    const auto almost_result = (temp * temp) % mod;

    return (degree % 2 == 0)
            ? almost_result
            : static_cast<cpp_int>((almost_result * x) % mod);
}

auto lab::is_fermat_prime(const cpp_int& x, int iters) -> bool
{
    if (x < 4) {
        return x == 2 || x == 3;
    }
    for (auto i = 0; i < iters; ++i) {
        auto a = random(2, (x - 2).convert_to<cpp_int>());
        if (pow(a, x - 1, x) != 1) {
            return false;
        }
    }
    return true;
}
auto lab::extended_euclid(const cpp_int& a, const cpp_int& b) -> lab::EuclidResult
{
    if (a == 0) {
        return {.gcd = b, .x = 0, .y = 1};
    }
    auto [gcd, x, y] = extended_euclid(b % a, a);
    return {.gcd = gcd, .x = y - (b / a) * x, .y = x};
}

auto lab::miller_rabin(const cpp_int& n, int iters) -> bool
{
    if (n == 2 || n == 3) {
        return true;
    }
    if (n < 2 || n % 2 == 0) {
        return false;
    }

    int s = 0;
    cpp_int t = n - 1;
    while (t % 2 == 0) {
        t /= 2;
        ++s;
    }
    for (auto i = 0; i < iters; ++i) {
        auto a = random(2, (n - 2).convert_to<cpp_int>());
        auto x = pow(a, t, n);
        if (x == 1 || x == n - 1) {
            continue;
        }
        for (auto _ = 1; _ < s; ++_) {
            x = x * x % n;
            if (x == 1) {
                return false;
            }
            if (x == n - 1) {
                break;
            }
        }
        if (x != n - 1) {
            return false;
        }
    }
    return true;
}

namespace {

    auto length (const cpp_int& num) -> std::size_t
    {
        return num.str().size();
    }
}
auto lab::karatsuba(const cpp_int& lhs, const cpp_int& rhs) -> cpp_int
{
    static constexpr std::size_t min_size = 10;
    const auto N = std::max(length(lhs), length(rhs));
    if (N < min_size) {
        return lhs * rhs;
    }
    const std::size_t half = std::floor(N / 2);
    const cpp_int ten_e_half = pow(cpp_int{10}, half);
    
    const cpp_int lhs_left = lhs / ten_e_half;
    const cpp_int lhs_right = lhs % ten_e_half;
    const cpp_int rhs_left = rhs / ten_e_half;
    const cpp_int rhs_right = rhs % ten_e_half;

    const std::array products = {
            karatsuba(lhs_left, rhs_left),
            karatsuba(lhs_right, rhs_right),
            karatsuba(lhs_left + lhs_right, rhs_left + rhs_right),
    };
    return products[0] * pow(cpp_int{10}, half * 2)
           + (products[2] - products[0] - products[1]) * ten_e_half
           + products[1];
}
