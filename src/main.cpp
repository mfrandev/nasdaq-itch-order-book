#include <fstream>
#include <vector>
#include <fmt/format.h>

#include <ProcessMessage.h>
#include <MessageHeader.h>

/**
 * Parse and calculate a VWAP for a binary NASDAQ ITCH 5.0 file
 */
int main() {

    // Open File    
    std::ifstream file("../01302019.NASDAQ_ITCH50", std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file");
        return 1;
    }

    // Initialize the buffer
    const std::size_t bufferSize = 64;  // 64 is a pretty ok buffer size. The largest message is 50 bytes, so why not leave a few more for good measure.
    std::vector<char> buffer(bufferSize);

    // And here is the magic
    uint64_t counter = 0;
    while(file) {
        // For some reason, there happens to be two leading bytes at the start of each line (maybe bad solution to another endienness problem ? it works though)
        file.read(buffer.data(), NUMBER_OF_BYTES_FOR_HEADER_CHUNK);
        std::shared_ptr<BinaryMessageHeader> header = parseHeader(&buffer[NUMBER_OF_BYTES_OFFSET_FOR_HEADER_CHUNK]);
        
        // if(header -> messageType == MESSAGE_TYPE_STOCK_TRADING_ACTION)
        //     fmt::println("{}. {} {} {} {}", ++counter, header -> messageType, header -> stockLocate, header -> trackingNumber, header -> timestamp);

        // Get the message body 
        std::size_t numberOfBytesForBody = messageTypeToNumberOfBytes(header -> messageType);
        file.read(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody);
        parseAndProcessMessageBody(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody, header);
        buffer.clear();
    }

    // Close file
    file.close();
    return 0;
}