#include "client.hpp"
#include "class/Flags.hpp"

Flags::Flags() : ip(-1), port(-1), isHelp(false) {}

void Flags::getFlags(int ac, char **av) {
    // case when no arguments are provided or too many arguments are provided
    if (ac < 2 || ac > 3) {
        printHelp(av[0]);
        exit(ERROR);
    }

    // case when the first argument is a help flag
    std::string arg(av[1]);
    if (arg == "-h" || arg == "--help") {
        isHelp = true;
        return;
    }

    // case when the correct number of arguments are provided
    if (ac == 3) {
        ip = std::stoi(arg);
        port = std::stoi(av[2]);
        return;
    } else {
        printHelp(av[0]);
        exit(ERROR);
    }
}

bool Flags::isHelpFlag() const { return isHelp; }

void Flags::printHelp(const char *progName) const { std::cout << "Usage: " << progName << " <ip> <port>" << std::endl; }

int Flags::getIp() const { return ip; }

int Flags::getPort() const { return port; }

void Flags::setIp(int ip) { this->ip = ip; }

void Flags::setPort(int port) { this->port = port; }