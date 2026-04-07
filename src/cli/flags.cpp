#include "client.hpp"
#include "class/Flags.hpp"

Flags::Flags() : ip(""), port(-1), isHelp(false) {}

Flags::~Flags() {}

void Flags::getFlags(int ac, char **av) {
    if (ac < 2 || ac > 3) {
        printHelp(av[0]);
        exit(ERROR);
    }

    std::string arg(av[1]);
    if (arg == "-h" || arg == "--help") {
        isHelp = true;
        return;
    }

    if (arg == "localhost")
        arg = "127.0.0.1";

    if (ac == 3) {
        try {
            ip = arg;
            port = std::stoi(av[2]);
        }
        catch (...) {
            printHelp(av[0]);
            exit(ERROR);
        }
        return;
    } else {
        printHelp(av[0]);
        exit(ERROR);
    }
}

bool Flags::isHelpFlag() const { return isHelp; }

void Flags::printHelp(const char *progName) const { std::cout << "Usage: " << progName << " <ip> <port>" << std::endl; }

const std::string &Flags::getIp() const { return ip; }

int Flags::getPort() const { return port; }

void Flags::setIp(const std::string &ip) { this->ip = ip; }

void Flags::setPort(int port) { this->port = port; }
