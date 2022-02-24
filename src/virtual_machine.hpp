#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>

#include "memory.hpp"
#include "registers.hpp"
#include "operations.hpp"
#include "utils.hpp"

class virtual_machine
{
    private:
	memory mem;
	registers regs;
	operations ops;

	static constexpr uintmax_t global_offset = 0x3000;

    public:
	virtual_machime(uintmax_t memory_size)
	    : mem(memory_size), regs(), ops(mem, regs) {}

	void load(std::string filename, uintmax_t offset)
	{
	    std::ifstream in(filename, std::ios::binary);
	    if(!in.is_open())
		throw std::runtime_error("Unalbe to open input file: " + filename);
	    uint16_t* p = mem.get_pointer() + global_offset + offset;
	    while(!in.eof())
		in.read(p++, sizeof(uint16_t));
	    in.close();
	}

	void run(uintmax_t offset)
	{
	    regs.get_RPC() = global_offset + offset;
	    while(true) {
		uint16_t instr = mem.read(regs.get_RPC()++);
		try {
		    ops.get_table()[utils::get_op_code(instr)](instr);
		} catch(std::runtime_error e) {
		    std::cout << e.what() << std::endl;
		}
	    }
	}
};
