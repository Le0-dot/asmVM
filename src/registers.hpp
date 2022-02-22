#pragma once

#include <vector>
#include <cstdint>

enum class registers_names {
    R0 = 0,
    R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    RPC, RCND, RCNT
};

class registers
{
    private:
	std::vector<uint16_t> regs;

    public:
	registers() 
	{
	    regs.reserve(static_cast<uint8_t>(registers_names::RCNT));
	    regs = {0};
	}

	uint16_t& get(uint8_t reg)
	{
	    return regs.at(reg);
	}

	uint16_t& get_RPC()
	{
	    return regs.at(static_cast<uint8_t>(registers_names::RPC));
	}

	uint16_t& get_RCND()
	{
	    return regs.at(static_cast<uint8_t>(registers_names::RCND));
	}
};
