#include <array>
#include <random>
#include <chrono>
#include <cstdint>

#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

template <std::size_t N>
struct RTEA {
    static_assert(N == 4 || N == 8, "Required size of RTEA key - 128 or 256 bits.");
    std::array<std::uint32_t, N> key;

    [[nodiscard]]
    constexpr auto encrypt(std::uint64_t x) const noexcept -> std::uint64_t
    {
        auto [a, b] = std::pair{std::uint32_t(x >> 32), std::uint32_t(x)};
        for (std::size_t i = 0; i < key.size() * 4 + 32; ++i) {
             b += a + ((a << 6) ^ (a >> 8)) + (key[i % key.size()] + i);
             ++i;
             a += b + ((b << 6) ^ (b >> 8)) + (key[i % key.size()] + i);
        }
        return std:: uint64_t{a} << 32 | b;
    }

     [[nodiscard]]
     constexpr auto decrypt(std::uint64_t x) const noexcept -> std::uint64_t
     {
         auto [a, b] = std::pair{std::uint32_t(x >> 32), std::uint32_t(x)};
         for (std::int64_t i = key.size() * 4 + 31; i >= 0; --i) {
              a -= b + ((b << 6) ^ (b >> 8)) + (key[i % key.size()] + i);
              --i;
              b -= a + ((a << 6) ^ (a >> 8)) + (key[i % key.size()] + i);
         }
         return std::uint64_t{a} << 32 | b;
     }
};

template <typename Int>
auto random_int() -> Int
{
    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<Int> dist{};
    return dist(gen);
}

int main() {
    namespace chr = std::chrono;

    constexpr auto rtea = RTEA<8>{      // we use 256-bit key
        .key = {0x20B94801, 0x84F22364, 0xA2A795BD, 0x7A69A953,
                0x7EFBC60F, 0xA2F47F3E, 0x23637D6, 0xAEDE318E}
    };

    auto print_and_check = [&] (auto x) {
        const auto start_time = chr::steady_clock::now();
        const auto crypted = rtea.encrypt(x);
        const auto decrypted = rtea.decrypt(crypted);
        const auto elapsed_time = chr::duration_cast<chr::nanoseconds>(chr::steady_clock::now() - start_time);

        fmt::print("Number: {}, encrypted: {}, decrypted: {}, elapsed: {}, ", x, crypted, decrypted, elapsed_time);
        if (x == decrypted) {
            fmt::print(fg(fmt::color::green), "correct\n");
        } else {
            fmt::print(fg(fmt::color::red), "incorrect\n");
        }
    };

    for (auto i = 0; i < 42; ++i) {
        print_and_check(random_int<std::uint64_t>());
    };

    return 0;
}
