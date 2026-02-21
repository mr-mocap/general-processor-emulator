#include "StringProcessing.hpp"
#include <charconv>
#include <system_error>


std::vector<std::string> BreakLine(std::string_view input)
{
    std::size_t final_position = 0;
    std::vector<std::string> retval;

    for ( std::pair<std::size_t, std::size_t> search_position = std::make_pair(0, 0);
        (search_position.second = input.find('\t', search_position.first)) != std::string_view::npos;
        search_position.first = search_position.second + 1, final_position = search_position.first
        )
    {
        retval.push_back(std::string{ input.substr(search_position.first, search_position.second - search_position.first) });
    }

    if ( final_position < input.size() )
        retval.push_back(std::string{ input.substr(final_position) });
    return retval;
}

std::vector<std::string_view> BreakLineView(std::string_view input)
{
    std::size_t final_position = 0;
    std::vector<std::string_view> retval;

    for ( std::pair<std::size_t, std::size_t> search_position = std::make_pair(0, 0);
        (search_position.second = input.find('\t', search_position.first)) != std::string_view::npos;
        search_position.first = search_position.second + 1, final_position = search_position.first
        )
    {
        retval.push_back( input.substr(search_position.first, search_position.second - search_position.first) );
    }

    if ( final_position < input.size() )
        retval.push_back( input.substr(final_position) );
    return retval;
}

std::vector<std::string> TrimRight(std::vector<std::string> &&line)
{
    if ( !line.empty() )
    {
        while ( line.back().empty() )
            line.pop_back();
    }
    return line;
}

std::vector<std::string_view> SplitString(std::string_view input, std::string_view delimiter)
{
    std::vector<std::string_view> result;

#if 0
    size_t start = 0;
    size_t end = input.find(delimiter, start);

    while (end != std::string_view::npos)
    {
        result.push_back( input.substr(start, end - start) );
        start = end + delimiter.size();
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
#else
    for ( size_t pos = input.find(delimiter);
          pos != std::string_view::npos;
          pos = input.find(delimiter) )
    {
        if ( pos == 0 )
        {
            result.push_back( input.substr(0, delimiter.size()) );
            input.remove_prefix( delimiter.size() );
        }
        else
        {
            result.push_back( input.substr(0, pos) );
            input.remove_prefix( pos );
        }
    }

    if ( !input.empty() )
        result.push_back( input );
#endif

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