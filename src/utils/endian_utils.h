#ifndef TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_
#define TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_

#include <cstdint>

/**
 * The binary file stores integers big-endian, and x86 is little endian.
 * Here is a utility function to convery big-endian to little-endian
 */
uint64_t toHostEndianUpTo64(const char* data, std::size_t size);

#endif // TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_