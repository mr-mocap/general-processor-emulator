#include "RegisterSet.hpp"
#include <vector>


bool RegisterSet::empty() const
{
    return _registers.empty();

}
std::optional<Register> RegisterSet::retrieveRegister(std::string_view name) const
{
    auto found = _registers.find( name );

    if ( found == _registers.end() )
        return std::nullopt;
    
    return std::make_optional( found->second );
}