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
    Register(std::string_view nm,
             int              bits = 8)
        :
        name(nm),
        number_of_bits(bits)
    {
    }
    Register(std::string_view nm,
             int              bits,
             std::string_view desc)
        :
        name(nm),
        description(desc),
        number_of_bits(bits)
    {
    }

    std::string name;
    std::string description;
    int         number_of_bits = 8;
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

    std::size_t size() const { return _registers.size(); }

    const_lookup_t retrieveRegister(std::string_view name) const;
protected:
    /** Use the "is_transparent" functionality to allow string_view to be used without constructing a temporary */
    std::map<std::string, Register, std::less<>> _registers;
};
