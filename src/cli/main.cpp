#include "client.hpp"
#include "class/Flags.hpp"

int main(int ac, char **av) {
    Flags flags;
    flags.getFlags(ac, av);

    if (flags.isHelpFlag()) {
        flags.printHelp(av[0]);
        return NOERROR;
    }

    Client client;
    if (!client.run(flags.getIp(), flags.getPort())) { return ERROR; }

    return NOERROR;
}
