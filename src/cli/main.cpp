#include "client.hpp"
#include "class/Flags.hpp"

int main(int ac, char **av) {
    Flags flags;
    flags.getFlags(ac, av);

    if (flags.isHelpFlag()) {
        flags.printHelp(av[0]);
        return NOERROR;
    }

    std::cout << "IP: " << flags.getIp() << std::endl;
    std::cout << "Port: " << flags.getPort() << std::endl;
    return NOERROR;
}