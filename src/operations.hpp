#pragma once

#include <cstdint>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <array>

#include <iostream>

#include "memory.hpp"
#include "registers.hpp"
#include "utils.hpp"

class operations
{
    private:
	memory& mem;
	registers& regs;

	static constexpr auto dr = utils::get_destination_register;
	static constexpr auto sr1 = utils::get_source_register1;
	static constexpr auto sr2 = utils::get_source_register2;
	static constexpr auto imm = utils::get_imm;
	static constexpr auto fimm = utils::get_fimm;
	static constexpr auto sextimm = utils::get_sextimm;
	static constexpr auto off6 = utils::get_offset6;
	static constexpr auto off9 = utils::get_offset9;
	static constexpr auto off11 = utils::get_offset11;
	static constexpr auto bit11 = utils::get_11bit;
	static constexpr auto bs = utils::get_baser;
	static constexpr auto trp = utils::get_trp;
	static constexpr auto fcnd = utils::get_fcnd;

	static constexpr uint16_t trap_offset = 0x20;

	static constexpr uint16_t FZ = 0b001;  
	static constexpr uint16_t FN = 0b010;  
	static constexpr uint16_t FP = 0b100;  


	void uf(uint8_t reg)
	{
	    if(regs[reg] == 0)
		regs.get_RCND() = FZ;
	    else if(regs[reg] >> 15)
		regs.get_RCND() = FN;
	    else
		regs.get_RCND() = FP;
	}


	void add(uint16_t instr)
	{
	    regs[dr(instr)] = regs[sr1(instr)] + 
		(fimm(instr) 
		 ? sextimm(instr) 
		 : regs[sr2(instr)]);
	    uf(dr(instr));
    	}

    	void and_op(uint16_t instr)
    	{
	    regs[dr(instr)] = regs[sr1(instr)] & 
		(fimm(instr) 
		 ? sextimm(instr) 
		 : regs[sr2(instr)]);
	    uf(dr(instr));
    	}

    	void ldi(uint16_t instr)
    	{
	    regs[dr(instr)] = mem.read(mem.read(regs.get_RPC() + off9(instr)));
	    uf(dr(instr));
    	}

    	void not_op(uint16_t instr)
    	{
	    regs[dr(instr)] = ~(regs[sr1(instr)]);
	    uf(dr(instr));
    	}

    	void br(uint16_t instr)
    	{
	    if(regs.get_RCND() & fcnd(instr))
		regs.get_RPC() += off9(instr);
    	}

    	void jsr(uint16_t instr)
    	{
	    regs[registers_names::R15] = regs.get_RPC(); // Remember rpc register into r15 (last one)
	    regs.get_RPC() = (bit11(instr)		  // and move rpc to new destination		    
		    ? regs.get_RPC() + off11(instr)
		    : bs(instr));
    	}

	void jmp(uint16_t instr)
    	{
	    regs.get_RPC() = regs[bs(instr)];
    	}

    	void ld(uint16_t instr)
    	{
	    regs[dr(instr)] = mem.read(regs.get_RPC() + off9(instr));
	    uf(dr(instr));
    	}

    	void ldr(uint16_t instr)
    	{
	    regs[dr(instr)] = mem.read(regs[sr1(instr)] + off6(instr));
	    uf(dr(instr));
    	}

    	void lea(uint16_t instr)
    	{
	    regs[dr(instr)] = regs[sr1(instr)] + off6(instr);
	    uf(dr(instr));
    	}

    	void st(uint16_t instr)
    	{
	    mem.write(regs.get_RPC() + off9(instr), regs[dr(instr)]);
    	}

    	void sti(uint16_t instr)
    	{
	    mem.write(mem.read(regs.get_RPC() + off9(instr)), regs[dr(instr)]);
    	}

    	void str(uint16_t instr)
    	{
	    mem.write(regs[sr1(instr)] + off9(instr), regs[dr(instr)]);
    	}

    	void rti(uint16_t instr)
    	{
    	}

    	void res(uint16_t instr)
    	{
    	}

    	void tgetc()
    	{
	    regs[registers_names::R0] = std::getchar();
    	}

    	void tout()
    	{
	    std::printf("%c", static_cast<char>(regs[registers_names::R0]));
    	}

    	void tputs()
    	{
	    uint16_t *p = mem.get_pointer() + regs[registers_names::R0];
	    for(; *p; printf("%c", static_cast<char>(*p)), p++);
    	}

    	void tin()
    	{
	    tgetc();
	    tout();
    	}

    	void tputsp()
    	{
    	}

    	void thalt()
    	{
	    throw std::runtime_error{"Halted"};
    	}

    	void tinu16()
    	{
	    std::scanf("%hu", &regs[registers_names::R0]);
    	}

    	void toutu16()
    	{
	    std::printf("%hu\n", regs[registers_names::R0]);
    	}

    	void trap(uint16_t instr)
    	{
	    std::array<decltype(std::bind(&operations::tgetc, *this)), 8> traps = {
		std::bind(&operations::tgetc, *this),
		std::bind(&operations::tout, *this),
		std::bind(&operations::tputs, *this),
		std::bind(&operations::tin, *this),
		std::bind(&operations::tputsp, *this),
		std::bind(&operations::thalt, *this),
		std::bind(&operations::tinu16, *this),
		std::bind(&operations::toutu16, *this)
	    };
	    traps[trp(instr) - trap_offset]();
    	}


    public:
	operations(memory& mem, registers& regs) 
	    : mem{mem}, regs{regs} {}

	auto get_table() -> std::array<decltype(std::bind(&operations::br, *this, std::placeholders::_1)), 16>
	{
	    return {
		std::bind(&operations::br, *this, std::placeholders::_1),
		std::bind(&operations::add, *this, std::placeholders::_1),
		std::bind(&operations::ld, *this, std::placeholders::_1),
		std::bind(&operations::st, *this, std::placeholders::_1),
		std::bind(&operations::jsr, *this, std::placeholders::_1),
		std::bind(&operations::and_op, *this, std::placeholders::_1),
		std::bind(&operations::ldr, *this, std::placeholders::_1),
		std::bind(&operations::str, *this, std::placeholders::_1),
		std::bind(&operations::rti, *this, std::placeholders::_1),
		std::bind(&operations::not_op, *this, std::placeholders::_1),
		std::bind(&operations::ldi, *this, std::placeholders::_1),
		std::bind(&operations::sti, *this, std::placeholders::_1),
		std::bind(&operations::jmp, *this, std::placeholders::_1),
		std::bind(&operations::res, *this, std::placeholders::_1),
		std::bind(&operations::lea, *this, std::placeholders::_1),
		std::bind(&operations::trap, *this, std::placeholders::_1)
	    };
	}
};
