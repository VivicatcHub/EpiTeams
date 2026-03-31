#pragma once

#include <string>

class Flags {
private:
    std::string ip;
    int port;
    bool isHelp;

public:
    Flags();
    ~Flags();
    void getFlags(int ac, char **av);
    bool isHelpFlag() const;
    void printHelp(const char *progName) const;

    const std::string &getIp() const;
    int getPort() const;

    void setIp(const std::string &ip);
    void setPort(int port);
};
