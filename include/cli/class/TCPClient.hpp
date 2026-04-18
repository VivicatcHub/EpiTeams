#pragma once

#include <string>

class TCPClient {
private:
    int _socket_fd;
    std::string _ip;
    int _port;

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
