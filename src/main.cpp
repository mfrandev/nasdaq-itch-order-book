#include <fstream>
#include <vector>
#include <fmt/format.h>

#include <ProcessMessage.h>
#include <MessageHeader.h>
#include <iostream>

/**
 * Parse and calculate a VWAP for a binary NASDAQ ITCH 5.0 file
 */
int main() {

    // TODO: CLA the filepath, default to this one
    // Open File    
    std::ifstream file("../01302019.NASDAQ_ITCH50", std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file");
        return 1;
    }

    // TODO: Make a circular buffer
    // Initialize the buffer
    const std::size_t bufferSize = 64;  // 64 is a pretty ok buffer size. The largest message is 50 bytes, so why not leave a few more for good measure.
    std::vector<char> buffer(bufferSize);

    // And here is the magic
    uint64_t counter = 0;
    while(file) {

        // TODO: Test another file to see if this pattern holds
        // For some reason, there happens to be two leading bytes at the start of each line (maybe bad solution to another endienness problem ? it works though)
        file.read(buffer.data(), NUMBER_OF_BYTES_FOR_HEADER_CHUNK);
        BinaryMessageHeader* header = parseHeader(&buffer[NUMBER_OF_BYTES_OFFSET_FOR_HEADER_CHUNK]);
        // std::cout << header << '\n';
        // if(header -> messageType == MESSAGE_TYPE_STOCK_TRADING_ACTION)
            // fmt::println("{}. {} {} {} {}", ++counter, (*header) -> messageType, (*header) -> stockLocate, (*header) -> trackingNumber, (*header) -> timestamp);

        // Get the message body 
        std::size_t numberOfBytesForBody = ProcessMessage::messageTypeToNumberOfBytes(header -> messageType);
        file.read(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody);

        // Highest level function call for maintaining books and calculating VWAP
        ProcessMessage::parseAndProcessMessageBody(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody, header);
        buffer.clear();
    }

    // Close file
    file.close();
    return 0;
}