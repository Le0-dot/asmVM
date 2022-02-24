#include <iostream>

#include "memory.hpp"
#include "operations.hpp"
#include "registers.hpp"
#include "utils.hpp"

int main()
{
    memory mem(10000000);
    registers regs;
    operations ops(mem, regs);
    auto table = ops.get_table();
    uint16_t t[] = {
	0xF026,
	0x1220,
	0xF026,
	0x1240,
	0x1060,
	0xF027,
	0xF025
    };
    for(const auto& it : t)
	table[utils::get_op_code(it)](it);
    return 0;
}
