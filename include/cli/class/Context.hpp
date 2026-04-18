#pragma once

#include <string>

class Context {
private:
    std::string _team_uuid;
    std::string _channel_uuid;
    std::string _thread_uuid;

public:
    Context();
    void setTeamUuid(const std::string &uuid);
    void setChannelUuid(const std::string &uuid);
    void setThreadUuid(const std::string &uuid);
    const std::string &getTeamUuid() const;
    const std::string &getChannelUuid() const;
    const std::string &getThreadUuid() const;
    void reset();
    bool isTeamContext() const;
    bool isChannelContext() const;
    bool isThreadContext() const;
};
