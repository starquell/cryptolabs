#include <array>
#include <cstdint>
#include <string_view>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/color.h>

#include <boost/crc.hpp>

class CRC32 {
public:
    explicit constexpr CRC32(std::uint32_t divisor)
    {
        auto const reversed_polynomial = std::uint_fast32_t{0xEDB88320uL};
        for (int i = 0; i < 256; ++i) {
            std::uint32_t crc = i;
            for (int j = 0; j < 8; ++j) {
                crc = crc & 1u ? (crc >> 1) ^ reversed_polynomial : crc >> 1;
            }
            _table[i] = crc;
        }
    }
    [[nodiscard]]
    constexpr auto calculate(std::string_view bytes) const noexcept -> std::uint32_t
    {
         auto crc = std::uint32_t {0xFFFFFFFFuL};
         for (const auto byte : bytes) {
             crc = _table[(crc ^ byte) & 0xFFu] ^ (crc >> 8);
         }
         return crc ^ 0xFFFFFFFFuL;
    }

private:
    std::array<std::uint32_t, 256> _table{};
};


int main() {

    constexpr auto crc = CRC32{0xEDB88320uL};     /// we`ll use CRC-32/BZIP2 (poly - 0x04C11DB7, initial - 0xFFFFFFFF)

    auto boost_crc = boost::crc_32_type{};

    constexpr std::array values = {
         "amogus",
         "aboba",
         "toyota"
    };

    for (std::size_t i = 0; i < values.size(); ++i) {
        const auto res = crc.calculate(values[i]);
        boost_crc.process_bytes(values[i], std::strlen(values[i]));

        fmt::print(R"(Input data = "{}", expected - "{:#X}", got - "{:#X}", )",
                   values[i], boost_crc.checksum(), res);
        if (boost_crc.checksum() == res) {
            fmt::print(fg(fmt::color::green), "ok!\n");
        } else {
            fmt::print(fg(fmt::color::red), "bad!\n");
        }
    }
    return 0;
}
