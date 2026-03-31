#pragma once

#include <string>

class TCPClient {
private:
    int socket_fd;
    std::string ip;
    int port;

public:
    TCPClient();
    ~TCPClient();
    bool connect(const std::string &ip, int port);
    bool send(const std::string &message);
    std::string receive();
    bool isConnected() const;
    int getSocketFd() const;
    void disconnect();
};
