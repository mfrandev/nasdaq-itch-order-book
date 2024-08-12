#ifndef TREXQUANTTAKEHOME_UTILS_STRING_UTILS_H_
#define TREXQUANTTAKEHOME_UTILS_STRING_UTILS_H_

#include <cstdint>
#include <string>
#include <algorithm>
#include <cctype>

/**
 * Although not necessary, I like knowing I have this utility
 */
std::string charStarToString(const char* data, size_t size);

/**
 * This utility removes all whitespace from a cpp string in-place
 */
void stripWhitespaceFromCPPString(std::string& str);

#endif // TREXQUANTTAKEHOME_UTILS_STRING_UTILS_H_