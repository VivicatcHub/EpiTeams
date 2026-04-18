#pragma once

#include "Context.hpp"
#include "LoggingHandler.hpp"
#include <vector>

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

class LogoutHandler : public LoggingHandler {
public:
    LogoutHandler() = default;
    virtual ~LogoutHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle231(const std::string &message) const;
    void __handle530() const;
};

class UsersHandler : public LoggingHandler {
public:
    UsersHandler() = default;
    virtual ~UsersHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle201(const std::string &message) const;
    void __handle530() const;
};

class UserHandler : public LoggingHandler {
public:
    UserHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~UserHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

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

class SubscribeHandler : public LoggingHandler {
public:
    SubscribeHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~SubscribeHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

class SubscribedHandler : public LoggingHandler {
public:
    SubscribedHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~SubscribedHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle201(const std::string &message) const;
    void __handle203(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

class UnsubscribeHandler : public LoggingHandler {
public:
    UnsubscribeHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~UnsubscribeHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

class CreateTeamHandler : public LoggingHandler {
public:
    CreateTeamHandler(const std::vector<std::string> &args) : _args(args) {}
    virtual ~CreateTeamHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    std::vector<std::string> _args;
    void __handle200(const std::string &message) const;
    void __handle504() const;
    void __handle530() const;
};

class CreateChannelHandler : public LoggingHandler {
public:
    CreateChannelHandler(const Context &context) : _context(context) {}
    virtual ~CreateChannelHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle504() const;
    void __handle530() const;
};

class CreateThreadHandler : public LoggingHandler {
public:
    CreateThreadHandler(const Context &context) : _context(context) {}
    virtual ~CreateThreadHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle504() const;
    void __handle530() const;
};

class CreateCommentHandler : public LoggingHandler {
public:
    CreateCommentHandler(const Context &context) : _context(context) {}
    virtual ~CreateCommentHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle200(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle504() const;
    void __handle530() const;
};

class ListTeamsHandler : public LoggingHandler {
public:
    ListTeamsHandler() = default;
    virtual ~ListTeamsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle201(const std::string &message) const;
    void __handle530() const;
};

class ListChannelsHandler : public LoggingHandler {
public:
    ListChannelsHandler(const Context &context) : _context(context) {}
    virtual ~ListChannelsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

class ListThreadsHandler : public LoggingHandler {
public:
    ListThreadsHandler(const Context &context) : _context(context) {}
    virtual ~ListThreadsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle530() const;
};

class ListCommentsHandler : public LoggingHandler {
public:
    ListCommentsHandler(const Context &context) : _context(context) {}
    virtual ~ListCommentsHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle530() const;
};

class InfoUserHandler : public LoggingHandler {
public:
    InfoUserHandler() = default;
    virtual ~InfoUserHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    void __handle201(const std::string &message) const;
    void __handle530() const;
};

class InfoTeamHandler : public LoggingHandler {
public:
    InfoTeamHandler(const Context &context) : _context(context) {}
    virtual ~InfoTeamHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle530() const;
};

class InfoChannelHandler : public LoggingHandler {
public:
    InfoChannelHandler(const Context &context) : _context(context) {}
    virtual ~InfoChannelHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle530() const;
};

class InfoThreadHandler : public LoggingHandler {
public:
    InfoThreadHandler(const Context &context) : _context(context) {}
    virtual ~InfoThreadHandler() = default;

    void handleResponse(const std::string &code, const std::string &message) const override;

private:
    Context _context;
    void __handle201(const std::string &message) const;
    void __handle404() const;
    void __handle405() const;
    void __handle406() const;
    void __handle530() const;
};
