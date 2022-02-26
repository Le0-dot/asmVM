#include "virtual_machine.hpp"

int main(int argc, char** argv)
{
    if(argc != 2) {
	std::cout << "Input program must be passed as argument. Aborting." << std::endl;
	return 1;
    }
    virtual_machine vm{};
    vm.load(argv[1]);
    vm.run();
    return 0;
}
