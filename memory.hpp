#pragma once

#include <vector>

class memory
{
    private:
	std::vector<uint16_t> mem;

    public:
	memory(const uintmax_t& size) : mem(size) {}

	uint16_t read(const uintmax_t& source)
	{
	    return mem.at(source);
	}

	void write(const uintmax_t& destination, uint16_t value)
	{
	    mem.at(destination) = value;
	}
};
