#pragma once

#include <string>
#include <string_view>
#include <vector>

/** Splits the input at delimiter and returns the resulting substrings
 * 
 *  @param input 
 *  @param delimiter A string_view containing the possibly multi-character delimiter to split the input at
 *
 *  @return A vector of string_views containing the substrings resulting from splitting the input at the delimiter
 * 
 *  @note The returned string_views point to the original input string, so the input string must remain
 *        valid as long as the returned string_views are used
 * 
 *  @note The delimiter is included in the returned substrings, so the resulting string_views will contain the delimiter as
 *        one of the substrings if it is present in the input string.
 */
std::vector<std::string_view> SplitString(std::string_view input, std::string_view delimiter);

struct ReadNumberResult
{
    int value = -1;
    std::string_view remaining_input;
};

ReadNumberResult ReadNumber(std::string_view input);