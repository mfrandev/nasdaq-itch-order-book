#include <BrokenTradeOrOrderExecution.h>

#include <cstring>
#include <cstdint>

/**
 * Parse the BrokenTradeOrOrderExecution body
 */
std::shared_ptr<BrokenTradeOrOrderExecution> parseBrokenTradeOrOrderExecutionBody(const char* data) {
    static std::shared_ptr<BrokenTradeOrOrderExecution> brokenTradeOrOrderExecution = std::make_shared<BrokenTradeOrOrderExecution>();
    std::size_t offset = 0;
    std::memcpy(&(brokenTradeOrOrderExecution -> matchNumber), &data[offset], 8); // We know this is an 8 byte int
    return brokenTradeOrOrderExecution;
}