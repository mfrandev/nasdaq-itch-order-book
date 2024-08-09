#include <string_utils.h>

/**
 * Although not necessary, I like knowing I have this utility
 */
std::string charStarToString(const char* data, std::size_t size) {
    return std::string(data, size);
}

/**
 * This utility removes all whitespace from a cpp string in-place
 */
void stripWhitespaceFromCPPString(std::string& str) {
    // Strip whitespace from stock
    str.erase(
        std::remove_if(
            str.begin(), 
            str.end(), ::isspace
        ), 
        str.end()
    );
}