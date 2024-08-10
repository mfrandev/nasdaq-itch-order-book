#include <BrokenTradeOrOrderExecution.h>

#include <cstring>
#include <cstdint>

#include <endian_utils.h>

/**
 * Parse the BrokenTradeOrOrderExecution body
 */
std::shared_ptr<BrokenTradeOrOrderExecution> parseBrokenTradeOrOrderExecutionBody(const char* data) {
    static std::shared_ptr<BrokenTradeOrOrderExecution> brokenTradeOrOrderExecution = std::make_shared<BrokenTradeOrOrderExecution>();
    std::size_t offset = 0;
    brokenTradeOrOrderExecution -> matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return brokenTradeOrOrderExecution;
}