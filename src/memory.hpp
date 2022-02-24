#pragma once

#include <vector>
#include <cstdint>

class memory
{
    private:
	std::vector<uint16_t> mem;

    public:
	memory(const uintmax_t& size) : mem(size, 0) {}

	uint16_t read(const uintmax_t& source)
	{
	    return mem.at(source);
	}

	void write(const uintmax_t& destination, uint16_t value)
	{
	    mem.at(destination) = value;
	}

	uint16_t* get_pointer()
	{
	    return mem.data();
	}
};
