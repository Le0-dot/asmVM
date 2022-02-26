#pragma once

#include <vector>
#include <cstdint>

class memory
{
    private:
	std::vector<uint16_t> mem;

    public:
	memory(const uint16_t size) : mem(size, 0) {}

	uint16_t read(const uint16_t source)
	{
	    return mem.at(source);
	}

	void write(const uint16_t destination, const uint16_t value)
	{
	    mem.at(destination) = value;
	}

	uint16_t* get_pointer()
	{
	    return mem.data();
	}
};
