#include <fstream>
#include <vector>
#include <fmt/format.h>
#include <chrono>
#include <string>

#include <ProcessMessage.h>
#include <MessageHeader.h>

#include <unordered_map>

/**
 * Parse and calculate a VWAP for a binary NASDAQ ITCH 5.0 file
 */
int main(int argc, char* argv[]) {

    if(argc > 2) {
        fmt::println("Do not specify more than 1 file. Quitting...");
        return 1;
    }
    std::string filepath("../ITCHFiles/01302019.NASDAQ_ITCH50");
    if(argc == 2) filepath = fmt::format("../ITCHFiles/{}",std::string(argv[1]));

    fmt::println("Using file: {}", filepath);

    auto start = std::chrono::system_clock::now();

    // Open File    
    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file. Quitting...");
        return 1;
    }

    // Initialize the buffer
    const std::size_t bufferSize = 64;  // 64 is a pretty ok buffer size. The largest message is 50 bytes, so why not leave a few more for good measure.
    std::vector<char> buffer(bufferSize);

    // And here is the magic
    uint64_t counter = 0;
    while(file) {

        // For some reason, there happens to be two leading bytes at the start of each line
        file.read(buffer.data(), NUMBER_OF_BYTES_FOR_HEADER_CHUNK);
        BinaryMessageHeader* header = parseHeader(&buffer[NUMBER_OF_BYTES_OFFSET_FOR_HEADER_CHUNK]);
        // fmt::println("{}. {} {} {} {}", ++counter, header -> messageType, header -> stockLocate, header -> trackingNumber, header -> timestamp);

        // Get the message body 
        std::size_t numberOfBytesForBody = ProcessMessage::messageTypeToNumberOfBytes(header -> messageType);
        file.read(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody);

        // Highest level function call for maintaining books and calculating VWAP
        ProcessMessage::parseAndProcessMessageBody(&buffer[NUMBER_OF_BYTES_FOR_HEADER_CHUNK], numberOfBytesForBody, header);
    }
    // Close file
    file.close();

    auto end = std::chrono::system_clock::now();
    // Convert nanos to seconds
    fmt::println("======== Total program execution time: {} seconds ========", static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()));

    return 0;
}