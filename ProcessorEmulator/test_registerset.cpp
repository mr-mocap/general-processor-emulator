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
        std::optional<Register> reg = rs.retrieveRegister("A");

        assert(reg.has_value());
        assert(reg.value().name == "A");
        assert(reg.value().number_of_bits == 8);
        assert(reg.value().internal == false);
        assert(reg.value().read_only == false);
    }

    {
        std::optional<Register> reg = rs.retrieveRegister("X");

        assert(reg.has_value());
        assert(reg.value().name == "X");
        assert(reg.value().number_of_bits == 16);
        assert(reg.value().internal == false);
        assert(reg.value().read_only == false);
    }

    {
        std::optional<Register> reg = rs.retrieveRegister("Y");

        assert(reg.has_value());
        assert(reg.value().name == "Y");
        assert(reg.value().number_of_bits == 8);
        assert(reg.value().internal == true);
        assert(reg.value().read_only == false);
    }

    {
        std::optional<Register> reg = rs.retrieveRegister("I");

        assert(reg.has_value());
        assert(reg.value().name == "I");
        assert(reg.value().number_of_bits == 8);
        assert(reg.value().internal == false);
        assert(reg.value().read_only == true);
    }

    {
        std::optional<Register> reg = rs.retrieveRegister("RO");

        assert(reg.has_value());
        assert(reg.value().name == "RO");
        assert(reg.value().number_of_bits == 8);
        assert(reg.value().internal == true);
        assert(reg.value().read_only == true);
    }
}

int main()
{
    DefaultConstructed();
    ConstructWithIterators();

    return EXIT_SUCCESS;
}
