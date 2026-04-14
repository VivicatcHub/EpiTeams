#pragma once

#include "LoggingHandler.hpp"
#include <vector>

/*
** LOGIN HANDLER
** Codes: 230 (success), 501 (incorrect user_name), 503 (already logged in), 540 (user_name already in use)
*/
class LoginHandler : public LoggingHandler {
public:
    LoginHandler() = default;
    virtual ~LoginHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle230(const std::string &message) const;
    void __handle501() const;
    void __handle503() const;
    void __handle540() const;
};

/*
** LOGOUT HANDLER
** Codes: 231 (success), 530 (not logged in)
*/
class LogoutHandler : public LoggingHandler {
public:
    LogoutHandler() = default;
    virtual ~LogoutHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle231(const std::string &message) const;
    void __handle530() const;
};

/*
** USERS HANDLER
** Codes: 200 (success, followed by users list), 530 (not logged in)
*/
class UsersHandler : public LoggingHandler {
public:
    UsersHandler() = default;
    virtual ~UsersHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle201(const std::string &message) const;
    void __handle530() const;
};

/*
** USER HANDLER
** Codes: 200 (success), 404 (user not found), 530 (not logged in)
*/
class UserHandler : public LoggingHandler {
public:
    UserHandler() = default;
    virtual ~UserHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle201(const std::string &message) const;
    void __handle404(const std::string &message) const;
    void __handle530() const;
};

/*
** SEND HANDLER
** Codes: 200 (success), 404 (user not found), 530 (not logged in)
*/
class SendHandler : public LoggingHandler {
public:
    SendHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~SendHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200() const;
    void __handle404() const;
    void __handle501() const;
    void __handle530() const;
};

/*
** MESSAGES HANDLER
** Codes: 200 (success, followed by messages list), 404 (user not found), 530 (not logged in)
*/
class MessagesHandler : public LoggingHandler {
public:
    MessagesHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~MessagesHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle501() const;
    void __handle530() const;
};

/*
** SUBSCRIBE HANDLER
** Codes: 200 (success), 404 (team not found), 530 (not logged in)
*/
class SubscribeHandler : public LoggingHandler {
public:
    SubscribeHandler() = default;
    virtual ~SubscribeHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200() const;
    void __handle404() const;
    void __handle530() const;
};

/*
** SUBSCRIBED HANDLER (list subscriptions)
** Codes: 200 (subscribed teams), 202 (team subscribers), 404 (team not found), 530 (not logged in)
*/
class SubscribedHandler : public LoggingHandler {
public:
    SubscribedHandler() = default;
    virtual ~SubscribedHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle202(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

/*
** UNSUBSCRIBE HANDLER
** Codes: 200 (success), 404 (team not found), 530 (not logged in)
*/
class UnsubscribeHandler : public LoggingHandler {
public:
    UnsubscribeHandler() = default;
    virtual ~UnsubscribeHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200() const;
    void __handle404() const;
    void __handle530() const;
};

/*
** CREATE_TEAM HANDLER
** Codes: 200 (success), 530 (not logged in)
*/
class CreateTeamHandler : public LoggingHandler {
public:
    CreateTeamHandler(const std::vector<std::string> &args) : _args(args) {
    }
    virtual ~CreateTeamHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle530() const;
};

/*
** CREATE_CHANNEL HANDLER
** Codes: 200 (success), 404 (team not found), 530 (not logged in)
*/
class CreateChannelHandler : public LoggingHandler {
public:
    CreateChannelHandler(const std::vector<std::string> &args) : _args(args) {
    }
    virtual ~CreateChannelHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

/*
** CREATE_THREAD HANDLER
** Codes: 200 (success), 404 (team or channel not found), 405 (channel not found), 530 (not logged in)
*/
class CreateThreadHandler : public LoggingHandler {
public:
    CreateThreadHandler(const std::vector<std::string> &args) : _args(args) {
    }
    virtual ~CreateThreadHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle530() const;
};

/*
** CREATE_COMMENT HANDLER (reply in thread)
** Codes: 200 (success), 404 (team, channel, or thread not found), 405 (channel not found), 406 (thread not found), 530 (not logged in)
*/
class CreateCommentHandler : public LoggingHandler {
public:
    CreateCommentHandler(const std::vector<std::string> &args) : _args(args) {
    }
    virtual ~CreateCommentHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200() const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle530() const;
};

/*
** LIST_TEAMS HANDLER
** Codes: 200 (success, followed by teams list), 530 (not logged in)
*/
class ListTeamsHandler : public LoggingHandler {
public:
    ListTeamsHandler() = default;
    virtual ~ListTeamsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle530() const;
};

/*
** LIST_CHANNELS HANDLER
** Codes: 200 (success, followed by channels list), 404 (team not found), 530 (not logged in)
*/
class ListChannelsHandler : public LoggingHandler {
public:
    ListChannelsHandler() = default;
    virtual ~ListChannelsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

/*
** LIST_THREADS HANDLER
** Codes: 200 (success, followed by threads list), 404 (team or channel not found), 405 (channel not found), 530 (not logged in)
*/
class ListThreadsHandler : public LoggingHandler {
public:
    ListThreadsHandler() = default;
    virtual ~ListThreadsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle530() const;
};

/*
** LIST_COMMENTS HANDLER
** Codes: 200 (success, followed by comments list), 404 (team, channel, or thread not found), 405 (channel not found), 406 (thread not found), 530 (not logged in)
*/
class ListCommentsHandler : public LoggingHandler {
public:
    ListCommentsHandler() = default;
    virtual ~ListCommentsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle530() const;
};

/*
** INFO_USER HANDLER
** Codes: 200 (success), 530 (not logged in)
*/
class InfoUserHandler : public LoggingHandler {
public:
    InfoUserHandler() = default;
    virtual ~InfoUserHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle530() const;
};

/*
** INFO_TEAM HANDLER
** Codes: 200 (success), 404 (team not found), 530 (not logged in)
*/
class InfoTeamHandler : public LoggingHandler {
public:
    InfoTeamHandler() = default;
    virtual ~InfoTeamHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

/*
** INFO_CHANNEL HANDLER
** Codes: 200 (success), 404 (team not found), 405 (channel not found), 530 (not logged in)
*/
class InfoChannelHandler : public LoggingHandler {
public:
    InfoChannelHandler() = default;
    virtual ~InfoChannelHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle530() const;
};

/*
** INFO_THREAD HANDLER
** Codes: 200 (success), 404 (team, channel or thread not found), 405 (channel not found), 406 (thread not found), 530 (not logged in)
*/
class InfoThreadHandler : public LoggingHandler {
public:
    InfoThreadHandler() = default;
    virtual ~InfoThreadHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle530() const;
};
