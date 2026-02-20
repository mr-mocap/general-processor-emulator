#include "StringProcessing.hpp"
#include <charconv>
#include <system_error>


std::vector<std::string_view> SplitString(std::string_view input, std::string_view delimiter)
{
    std::vector<std::string_view> result;

    size_t start = 0;
    size_t end = input.find(delimiter, start);

    while (end != std::string_view::npos)
    {
        result.push_back( input.substr(start, end - start) );
        start = end + delimiter.length();
        end = input.find( delimiter, start );
    }

    // Add the last substring after the final delimiter
    result.push_back( input.substr(start) );

    while ( !input.empty() )
    {
        size_t pos = input.find(delimiter);

        if (pos == std::string_view::npos)
        {
            // Remove the matched part...
            result.push_back(input);
            input.remove_prefix( input.size() );
        }
        else
        {
            if (pos > 0)
            {
                // Remove the matched part...
                result.push_back( input.substr(0, pos) );
                input.remove_prefix( pos );
            }
            // Remove the delimiter itself
            input.remove_prefix( delimiter.size() );
            result.push_back( delimiter );
        }
    }

    return result;
}

ReadNumberResult ReadNumber(std::string_view input)
{
    int base = 10;

    // Check for base prefix and remove it if present
    if ( input.starts_with('$') ) // Hexadecimal prefix
    {
        input.remove_prefix(1);
        base = 16;
    }
    else if ( input.starts_with('%') ) // Binary prefix
    {
        input.remove_prefix(1);
        base = 2;
    }
    else if ( input.starts_with('o') ) // Octal prefix
    {
        input.remove_prefix(1);
        base = 8;
    }

    size_t retval;
    auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), retval, base);

    if ( ec == std::errc() )
    {
        // SUCCESS!
        ReadNumberResult result;

        result.value = static_cast<int>(retval);

        // If ptr is NOT at the end of the input, we have some remaining input after the number,
        // so we need to return that as well.
        if ( ptr != input.data() + input.size() )
        {
            input.remove_prefix( ptr - input.data() );
            result.remaining_input = input;
        }
        return result;
    }

    return {}; // Failure
}