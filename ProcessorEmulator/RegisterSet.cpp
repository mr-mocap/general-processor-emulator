#include "RegisterSet.hpp"
#include <vector>


bool RegisterSet::empty() const
{
    return _registers.empty();

}
RegisterSet::const_lookup_t RegisterSet::retrieveRegister(std::string_view name) const
{
    auto found = _registers.find( name );

    if ( found == _registers.end() )
        return std::nullopt;
    
    return std::make_optional( std::ref(found->second) );
}