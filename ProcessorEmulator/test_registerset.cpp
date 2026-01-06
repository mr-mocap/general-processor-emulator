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
        {"A", 8, false, false},
        {"X", 16, false, false},
        {"Y", 8, true, false},
        {"I", 8, false, true},
        {"RO", 8, true, true}
    };
    RegisterSet rs( registers );

    assert(!rs.empty());

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("A");

        assert(reg.has_value());
        assert(reg.value().get().name == "A");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().internal == false);
        assert(reg.value().get().read_only == false);
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("X");

        assert(reg.has_value());
        assert(reg.value().get().name == "X");
        assert(reg.value().get().number_of_bits == 16);
        assert(reg.value().get().internal == false);
        assert(reg.value().get().read_only == false);
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("Y");

        assert(reg.has_value());
        assert(reg.value().get().name == "Y");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().internal == true);
        assert(reg.value().get().read_only == false);
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("I");

        assert(reg.has_value());
        assert(reg.value().get().name == "I");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().internal == false);
        assert(reg.value().get().read_only == true);
    }

    {
        RegisterSet::const_lookup_t reg = rs.retrieveRegister("RO");

        assert(reg.has_value());
        assert(reg.value().get().name == "RO");
        assert(reg.value().get().number_of_bits == 8);
        assert(reg.value().get().internal == true);
        assert(reg.value().get().read_only == true);
    }
}

int main()
{
    DefaultConstructed();
    ConstructWithIterators();

    return EXIT_SUCCESS;
}
