#include <iostream>
#include <fstream>
#include <vector>

#include <fmt/format.h>

#include <messages/common.h>
#include <endian_utils.h>


int main() {
    // Open File    
    const std::size_t bufferSize = 64;  // 64 is a pretty safe buffer size. The largest message is 50 bytes, so why not leave a few more for good measure.
    std::ifstream file("../01302019.NASDAQ_ITCH50", std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file");
        return 1;
    }

    // Initialize the buffer
    std::vector<char> buffer(bufferSize);
    uint64_t count = 0;

    // And here is the magic
    while(file) {
        ++count;
        // For some reason, there happens to be two leading bytes at the start of each line (maybe bad solution to another endienness problem ?)
        file.read(buffer.data(), 13);
        std::shared_ptr<BinaryMessageHeader> header = parseHeader(&buffer[2]);
        
        // fmt::println("\n{} {} {} {}", header -> message_type, header -> stock_locate, header -> tracking_number, header -> timestamp);

        // Get the message body 
        file.read(buffer.data(), messageTypeTo(header -> message_type));
        buffer.clear();
    }

    // Close file
    file.close();
    return 0;
}