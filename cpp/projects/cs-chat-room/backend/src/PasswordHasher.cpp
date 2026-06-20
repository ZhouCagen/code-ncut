#include "PasswordHasher.hpp"

#include <array>
#include <cstddef>
#include <iomanip>
#include <openssl/evp.h>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
std::string bytesToHex(const unsigned char *data, std::size_t size)
{
    std::ostringstream stream;

    for (std::size_t i = 0; i < size; i++)
    {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }

    return stream.str();
}
} // namespace

std::string PasswordHasher::hashPassword(std::string_view password)
{
    return md5Hex(password);
}

bool PasswordHasher::verifyPassword(std::string_view password, std::string_view storedHash)
{
    return md5Hex(password) == storedHash;
}

std::string PasswordHasher::md5Hex(std::string_view input)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
    unsigned int hashLength = 0;

    int result =
        EVP_Digest(input.data(), input.size(), hash.data(), &hashLength, EVP_md5(), nullptr);

    if (result != 1)
        throw std::runtime_error("MD5 failed");

    return bytesToHex(hash.data(), hashLength);
}
