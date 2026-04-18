#include "client.hpp"
#include "class/Flags.hpp"

Flags::Flags() : _ip(""), _port(-1), _isHelp(false) {}

Flags::~Flags() {}

void Flags::getFlags(int ac, char **av) {
    if (ac < 2 || ac > 3) {
        printHelp(av[0]);
        exit(ERROR);
    }

    std::string arg(av[1]);
    if (arg == "-h" || arg == "--help") {
        _isHelp = true;
        return;
    }

    if (arg == "localhost")
        arg = "127.0.0.1";

    if (ac == 3) {
        try {
            _ip = arg;
            _port = std::stoi(av[2]);
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

bool Flags::isHelpFlag() const { return _isHelp; }

void Flags::printHelp(const char *progName) const { std::cout << "Usage: " << progName << " <ip> <port>" << std::endl; }

const std::string &Flags::getIp() const { return _ip; }

int Flags::getPort() const { return _port; }

void Flags::setIp(const std::string &ip) { this->_ip = ip; }

void Flags::setPort(int port) { this->_port = port; }
