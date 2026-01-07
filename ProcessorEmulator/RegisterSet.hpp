#pragma once

#include <string>
#include <string_view>
#include <map>
#include <optional>
#include <ranges>
#include <functional>


struct Register
{
    Register() = default;
    Register(std::string_view n,
             int              bits = 8,
             bool             i = false,
             bool             ro = false)
        :
        name(n),
        number_of_bits(bits),
        internal(i),
        read_only(ro)
    {
    }

    std::string name;
    int         number_of_bits = 8;
    bool        internal  = false;
    bool        read_only = false;
};


class RegisterSet
{
public:
    using const_lookup_t = std::optional<std::reference_wrapper<const Register>>;

    RegisterSet() = default;
    
    template <std::ranges::range RangeType>
    RegisterSet(RangeType &r)
    {
        for (const Register &iCurrentValue : r)
            _registers.insert_or_assign( iCurrentValue.name, iCurrentValue );
    }

    bool empty() const;

    const_lookup_t retrieveRegister(std::string_view name) const;
protected:
    /** Use the "is_transparent" functionality to allow string_view to be used without constructing a temporary */
    std::map<std::string, Register, std::less<>> _registers;
};
