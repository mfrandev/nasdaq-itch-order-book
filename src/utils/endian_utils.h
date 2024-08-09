#ifndef TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_
#define TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_

#include <cstdint>

uint64_t toHostEndianUpTo64(const char* data, std::size_t size);

#endif // TREXQUANTTAKEHOME_UTILS_ENDIAN_UTILS_H_