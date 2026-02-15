#include "RegisterSet.hpp"
#include <cstdlib>
#include <vector>
#include <cassert>


void DefaultConstructed()
{
    RegisterSet rs;

    assert(rs.empty());
}

void ConstructWithIterators()
{
    std::vector<Register> registers{
        {"A", 8, "Accumulator" },
        {"X", 16, "X Register" },
        {"Y", 8, "Y Register" }
    };
    RegisterSet rs( registers );

    assert(!rs.empty());
    assert( rs.size() == registers.size() );

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("A");

        assert(reg.has_value());
        assert(reg.value().get().name == "A");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().description == "Accumulator");
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("X");

        assert(reg.has_value());
        assert(reg.value().get().name == "X");
        assert(reg.value().get().number_of_bits == 16);
        assert(reg.value().get().description == "X Register");
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("Y");

        assert(reg.has_value());
        assert(reg.value().get().name == "Y");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().description == "Y Register");
    }
}

int main()
{
    DefaultConstructed();
    ConstructWithIterators();

    return EXIT_SUCCESS;
}
