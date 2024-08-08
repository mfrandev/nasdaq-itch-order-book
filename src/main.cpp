#include <iostream>
#include <fstream>
#include <vector>

#include <fmt/format.h>

#include <messages/common.h>

int main() {
    // Open File
    const std::size_t n = 64;  // Number of bytes to read
    BinaryMessageHeader header = {};
    std::ifstream file("data.bin", std::ios::binary);

    if (!file) {
        fmt::println("Failed to open the file");
        return 1;
    }

    // Loop all file contents
    std::vector<char> buf(n);
    while(file) {
        file.read(buf.data(), 11);
        std::memcpy(&header, &buf[0], 11);
        fmt::format("Message Type: {}, Stock Locate: {}, Tracking Number: {}, Timestamp: {}", 
        header.message_type, header.stock_locate, header.tracking_number, header.timestamp);
        file.read(buf.data(), messageTypeTo(header.message_type));
        buf.clear();
    }

    // Close file
    file.close();
    return 0;
}