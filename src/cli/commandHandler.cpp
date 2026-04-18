#include "class/CommandHandler.hpp"
#include <iostream>
#include <sstream>

CommandHandler::CommandHandler(TCPClient &client, Context &ctx) : _tcp_client(client), _context(ctx) {}

bool CommandHandler::__sendCommand(const std::string &command) {
    if (!_tcp_client.isConnected()) { return false; }
    return _tcp_client.send(command + "\r\n");
}

bool CommandHandler::__handleHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "/help - Display this help message" << std::endl;
    std::cout << "/login \"user_name\" - Login to the server" << std::endl;
    std::cout << "/logout - Logout from the server" << std::endl;
    std::cout << "/users - List all users" << std::endl;
    std::cout << "/user \"user_uuid\" - Get user info" << std::endl;
    std::cout << "/send \"user_uuid\" \"message\" - Send private message" << std::endl;
    std::cout << "/messages \"user_uuid\" - List messages with user" << std::endl;
    std::cout << "/subscribe \"team_uuid\" - Subscribe to team" << std::endl;
    std::cout << "/unsubscribe \"team_uuid\" - Unsubscribe from team" << std::endl;
    std::cout << "/subscribed - List subscribed teams" << std::endl;
    std::cout << "/use [team_uuid [channel_uuid [thread_uuid]]] - Set context" << std::endl;
    std::cout << "/create \"...\" - Create team/channel/thread/comment" << std::endl;
    std::cout << "/list - List teams/channels/threads/comments" << std::endl;
    std::cout << "/info - Get current user/team/channel/thread info" << std::endl;
    std::cout << "/quit - Exit the client" << std::endl;
    std::cout << "/exit - Exit the client" << std::endl;
    return true;
}

bool CommandHandler::__handleLogin(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "LOGIN " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleLogout() { return __sendCommand("LOGOUT"); }

bool CommandHandler::__handleUsers() { return __sendCommand("USERS"); }

bool CommandHandler::__handleUser(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "USER " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleSend(const std::vector<std::string> &args) {
    if (args.size() != 2) { return false; }
    std::string cmd = "SEND " + args[0] + " " + args[1];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleMessages(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "MESSAGES " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleSubscribe(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "SUBSCRIBE " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleUnsubscribe(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "UNSUBSCRIBE " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleSubscribed(const std::vector<std::string> &args) {
    if (args.size() == 0) { return __sendCommand("SUBSCRIBED"); }
    if (args.size() == 1) {
        std::string cmd = "SUBSCRIBED " + args[0];
        return __sendCommand(cmd);
    }
    return false;
}

bool CommandHandler::__handleUse(const std::vector<std::string> &args) {
    if (args.size() == 0) {
        _context.reset();
        return true;
    }

    if (args.size() > 3) return false;

    if (args.size() >= 1) { _context.setTeamUuid(args[0]); } else { _context.setTeamUuid(""); }
    if (args.size() >= 2) { _context.setChannelUuid(args[1]); } else { _context.setChannelUuid(""); }
    if (args.size() >= 3) { _context.setThreadUuid(args[2]); } else { _context.setThreadUuid(""); }
    return args.size() <= 3;
}

bool CommandHandler::__handleCreate(const std::vector<std::string> &args) {
    if (args.size() == 0) { return false; }
    if (!_context.isTeamContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_TEAM " + args[0] + " " + args[1];
        return __sendCommand(cmd);
    }

    if (!_context.isChannelContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_CHANNEL " + _context.getTeamUuid() + " " + args[0] + " " + args[1];
        return __sendCommand(cmd);
    }

    if (!_context.isThreadContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_THREAD " + _context.getTeamUuid() + " " + _context.getChannelUuid() + " " + args[0] + " " + args[1];
        return __sendCommand(cmd);
    }

    if (args.size() != 1) { return false; }
    std::string cmd = "CREATE_COMMENT " + _context.getTeamUuid() + " " + _context.getChannelUuid() + " " + _context.getThreadUuid() + " " + args[0];
    return __sendCommand(cmd);
}

bool CommandHandler::__handleList() {
    if (!_context.isTeamContext()) { return __sendCommand("LIST_TEAMS"); }

    if (!_context.isChannelContext()) {
        std::string cmd = "LIST_CHANNELS " + _context.getTeamUuid();
        return __sendCommand(cmd);
    }

    if (!_context.isThreadContext()) {
        std::string cmd = "LIST_THREADS " + _context.getTeamUuid() + " " + _context.getChannelUuid();
        return __sendCommand(cmd);
    }

    std::string cmd = "LIST_COMMENTS " + _context.getTeamUuid() + " " + _context.getChannelUuid() + " " + _context.getThreadUuid();
    return __sendCommand(cmd);
}

bool CommandHandler::__handleInfo(const std::vector<std::string> &args) {
    if (!_context.isTeamContext()) {
        if (args.size() == 0) return __sendCommand("INFO_USER");
        if (args.size() == 1) {
            std::string cmd = "INFO_USER " + args[0];
            return __sendCommand(cmd);
        }
        return false;
    }

    if (!_context.isChannelContext()) {
        std::string cmd = "INFO_TEAM " + _context.getTeamUuid();
        return __sendCommand(cmd);
    }

    if (!_context.isThreadContext()) {
        std::string cmd = "INFO_CHANNEL " + _context.getTeamUuid() + " " + _context.getChannelUuid();
        return __sendCommand(cmd);
    }

    std::string cmd = "INFO_THREAD " + _context.getTeamUuid() + " " + _context.getChannelUuid() + " " + _context.getThreadUuid();
    return __sendCommand(cmd);
}

bool CommandHandler::execute(const std::string &command, const std::vector<std::string> &args) {
    if (command == "help") { return __handleHelp(); }
    if (command == "login") { return __handleLogin(args); }
    if (command == "logout") { return __handleLogout(); }
    if (command == "users") { return __handleUsers(); }
    if (command == "user") { return __handleUser(args); }
    if (command == "send") { return __handleSend(args); }
    if (command == "messages") { return __handleMessages(args); }
    if (command == "subscribe") { return __handleSubscribe(args); }
    if (command == "unsubscribe") { return __handleUnsubscribe(args); }
    if (command == "subscribed") { return __handleSubscribed(args); }
    if (command == "use") { return __handleUse(args); }
    if (command == "create") { return __handleCreate(args); }
    if (command == "list") { return __handleList(); }
    if (command == "info") { return __handleInfo(args); }
    if (command == "laugh") { return __sendCommand("LAUGH"); }

    return false;
}
