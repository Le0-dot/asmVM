#pragma once

#include <vector>
#include <cstdint>

enum class registers_names {
    R0 = 0,
    R1, R2, R3, R4, R5, R6, R7, R8,
    RPC, RCND, RCNT
};

class registers
{
    private:
	std::vector<uint16_t> regs;

    public:
	registers() : regs(static_cast<uint8_t>(registers_names::RCNT), 0) {}

	uint16_t& operator[](uint8_t reg)
	{
	    return regs.at(reg);
	}

	uint16_t& operator[](registers_names reg_name)
	{
	    return regs[static_cast<uint8_t>(reg_name)];
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
