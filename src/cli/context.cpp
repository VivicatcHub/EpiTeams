#include "class/Context.hpp"

Context::Context() {}

void Context::setTeamUuid(const std::string &uuid) { team_uuid = uuid; }

void Context::setChannelUuid(const std::string &uuid) { channel_uuid = uuid; }

void Context::setThreadUuid(const std::string &uuid) { thread_uuid = uuid; }

const std::string &Context::getTeamUuid() const { return team_uuid; }

const std::string &Context::getChannelUuid() const { return channel_uuid; }

const std::string &Context::getThreadUuid() const { return thread_uuid; }

void Context::reset() {
    team_uuid.clear();
    channel_uuid.clear();
    thread_uuid.clear();
}

bool Context::isTeamContext() const { return !team_uuid.empty(); }

bool Context::isChannelContext() const { return !team_uuid.empty() && !channel_uuid.empty(); }

bool Context::isThreadContext() const { return !team_uuid.empty() && !channel_uuid.empty() && !thread_uuid.empty(); }
