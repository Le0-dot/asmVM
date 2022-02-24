#pragma once

#include <cstdint>

namespace utils
{
    static inline uint16_t sext(uint16_t n, int b) {
	// convert number with 'b' bits signed to 16 bits signed
	return ((n >> (b - 1)) & 1) ? // if bit with index 'b' is 0
	    (n | (0xFFFF << b))	      // then we just fill all other bits with 1 ( 1111 1111 1111 xxxx )
				      //							^
				      //							|
				      //							b
	    : n;		      // else we just return the value
    }

    inline uint16_t get_op_code(uint16_t instruction)
    {
	return instruction >> 12;
    }

    inline uint16_t get_destination_register(uint16_t instruction)
    {
	return (instruction >> 9) & 0b111;
    }

    inline uint16_t get_source_register1(uint16_t instruction)
    {
	return (instruction >> 6) & 0b111;
    }

    inline uint16_t get_source_register2(uint16_t instruction)
    {
	return instruction & 0b111;
    }

    inline uint16_t get_imm(uint16_t instruction)
    {
	return instruction & 0b11111;
    }

    inline uint16_t get_fimm(uint16_t instruction)
    {
	return (instruction >> 5) & 0b1;
    }

    inline uint16_t get_sextimm(uint16_t instruction)
    {
	return sext(get_imm(instruction), 5);
    }

    inline uint16_t get_offset6(uint16_t instruction)
    {
	return sext(instruction & 0b111111, 6);
    }

    inline uint16_t get_offset9(uint16_t instruction)
    {
	return sext(instruction & 0b111111111, 9);
    }

    inline uint16_t get_offset11(uint16_t instruction)
    {
	return sext(instruction & 0b11111111111, 11);
    }

    inline uint16_t get_11bit(uint16_t instruction)
    {
	return (instruction >> 11) & 1;
    }

    inline uint16_t get_baser(uint16_t instruction)
    {
	return (instruction >> 6) & 0b111;
    }
    
    inline uint16_t get_trp(uint16_t instruction)
    {
	return instruction & 0b11111111;
    }

    inline uint16_t get_fcnd(uint16_t instruction)
    {
	return (instruction >> 9) & 0b111;
    }
}
