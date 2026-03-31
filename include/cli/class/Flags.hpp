class Flags {
private:
    int ip;
    int port;
    bool isHelp;

public:
    Flags();
    void getFlags(int ac, char **av);
    bool isHelpFlag() const;
    void printHelp(const char *progName) const;
    // Getters
    int getIp() const;
    int getPort() const;
    // Setters
    void setIp(int ip);
    void setPort(int port);
};
