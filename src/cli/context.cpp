#include "class/Context.hpp"

Context::Context() {}

void Context::setTeamUuid(const std::string &uuid) { _team_uuid = uuid; }

void Context::setChannelUuid(const std::string &uuid) { _channel_uuid = uuid; }

void Context::setThreadUuid(const std::string &uuid) { _thread_uuid = uuid; }

const std::string &Context::getTeamUuid() const { return _team_uuid; }

const std::string &Context::getChannelUuid() const { return _channel_uuid; }

const std::string &Context::getThreadUuid() const { return _thread_uuid; }

void Context::reset() {
    _team_uuid.clear();
    _channel_uuid.clear();
    _thread_uuid.clear();
}

bool Context::isTeamContext() const { return !_team_uuid.empty(); }

bool Context::isChannelContext() const { return !_team_uuid.empty() && !_channel_uuid.empty(); }

bool Context::isThreadContext() const { return !_team_uuid.empty() && !_channel_uuid.empty() && !_thread_uuid.empty(); }
