#include <BrokenTradeOrOrderExecution.h>

#include <endian_utils.h>

/**
 * Parse the BrokenTradeOrOrderExecution body
 */
BrokenTradeOrOrderExecution parseBrokenTradeOrOrderExecutionBody(const char* data) {
    size_t offset = 0;
    uint64_t matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return BrokenTradeOrOrderExecution(matchNumber);
}
