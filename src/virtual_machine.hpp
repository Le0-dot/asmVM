#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <filesystem>

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

	static constexpr uint16_t global_offset = 0x3000;

    public:
	virtual_machine(uint16_t memory_size = UINT16_MAX)
	    : mem(memory_size), regs(), ops(mem, regs) {}

	void load(std::filesystem::path filename, uint16_t offset = 0)
	{
	    if(!std::filesystem::exists(filename))
		throw std::runtime_error("File " + filename.native() + " does not exists");
	    if(std::filesystem::file_size(filename) % 2)
		throw std::runtime_error("File " + filename.native() + " is damaged and cannot be read");
	    std::ifstream in(filename, std::ios::binary);
	    if(!in.is_open())
		throw std::runtime_error("Unalbe to open " + filename.native());
	    uint16_t* p = mem.get_pointer() + global_offset + offset;
	    for(; in.read(reinterpret_cast<char*>(p), sizeof(*p)); p++);
	    std::cerr << "Program loaded. Bytes loaded: " << (p - mem.get_pointer() - global_offset - offset) * 2 << std::endl;
	}

	void run(uint16_t offset = 0)
	{
	    std::cerr << "Start running the program." << std::endl;
	    regs.get_RPC() = global_offset + offset;
	    while(true) {
		uint16_t instr = mem.read(regs.get_RPC()++);
		try {
		    ops.get_table()[utils::get_op_code(instr)](instr);
		} catch(std::runtime_error e) {
		    std::cerr << e.what() << std::endl;
		    return;
		}
	    }
	}
};
