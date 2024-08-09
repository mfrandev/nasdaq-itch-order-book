#include <endian_utils.h>

/**
 * The binary file stores integers big-endian, and x86 is little endian.
 * Here is a utility function to convery big-endian to little-endian
 */
uint64_t toHostEndianUpTo64(const char* data, std::size_t size) {
    uint64_t result = 0;

    // shift to make the LSB of "data" the MSB of "result"
    for (std::size_t i = 0; i < size; ++i) {
        result |= static_cast<uint64_t>(static_cast<uint8_t>(data[i])) << ((size - 1 - i) * 8);
    }
    return result;
}