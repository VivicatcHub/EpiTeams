#include "class/CommandHandler.hpp"
#include <iostream>
#include <sstream>

CommandHandler::CommandHandler(TCPClient &client, Context &ctx) : tcp_client(client), context(ctx) {}

bool CommandHandler::sendCommand(const std::string &command) {
    if (!tcp_client.isConnected()) { return false; }
    return tcp_client.send(command + "\r\n");
}

bool CommandHandler::handleHelp() {
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
    return true;
}

bool CommandHandler::handleLogin(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "LOGIN " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleLogout() { return sendCommand("LOGOUT"); }

bool CommandHandler::handleUsers() { return sendCommand("USERS"); }

bool CommandHandler::handleUser(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "USER " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleSend(const std::vector<std::string> &args) {
    if (args.size() != 2) { return false; }
    std::string cmd = "SEND " + args[0] + " " + args[1];
    return sendCommand(cmd);
}

bool CommandHandler::handleMessages(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "MESSAGES " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleSubscribe(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "SUBSCRIBE " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleUnsubscribe(const std::vector<std::string> &args) {
    if (args.size() != 1) { return false; }
    std::string cmd = "UNSUBSCRIBE " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleSubscribed(const std::vector<std::string> &args) {
    if (args.size() == 0) { return sendCommand("SUBSCRIBED"); }
    if (args.size() == 1) {
        std::string cmd = "SUBSCRIBED " + args[0];
        return sendCommand(cmd);
    }
    return false;
}

bool CommandHandler::handleUse(const std::vector<std::string> &args) {
    if (args.size() == 0) {
        context.reset();
        return true;
    }

    if (args.size() >= 1) { context.setTeamUuid(args[0]); }
    if (args.size() >= 2) { context.setChannelUuid(args[1]); }
    if (args.size() >= 3) { context.setThreadUuid(args[2]); }
    return args.size() <= 3;
}

bool CommandHandler::handleCreate(const std::vector<std::string> &args) {
    if (args.size() == 0) { return false; }
    if (!context.isTeamContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_TEAM " + args[0] + " " + args[1];
        return sendCommand(cmd);
    }

    if (!context.isChannelContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_CHANNEL " + context.getTeamUuid() + " " + args[0] + " " + args[1];
        return sendCommand(cmd);
    }

    if (!context.isThreadContext()) {
        if (args.size() != 2) { return false; }
        std::string cmd = "CREATE_THREAD " + context.getChannelUuid() + " " + args[0] + " " + args[1];
        return sendCommand(cmd);
    }

    if (args.size() != 1) { return false; }
    std::string cmd = "CREATE_COMMENT " + context.getThreadUuid() + " " + args[0];
    return sendCommand(cmd);
}

bool CommandHandler::handleList() {
    if (!context.isTeamContext()) { return sendCommand("LIST_TEAMS"); }

    if (!context.isChannelContext()) {
        std::string cmd = "LIST_CHANNELS " + context.getTeamUuid();
        return sendCommand(cmd);
    }

    if (!context.isThreadContext()) {
        std::string cmd = "LIST_THREADS " + context.getChannelUuid();
        return sendCommand(cmd);
    }

    std::string cmd = "LIST_COMMENTS " + context.getThreadUuid();
    return sendCommand(cmd);
}

bool CommandHandler::handleInfo() {
    if (!context.isTeamContext()) { return sendCommand("INFO_USER"); }

    if (!context.isChannelContext()) {
        std::string cmd = "INFO_TEAM " + context.getTeamUuid();
        return sendCommand(cmd);
    }

    if (!context.isThreadContext()) {
        std::string cmd = "INFO_CHANNEL " + context.getChannelUuid();
        return sendCommand(cmd);
    }

    std::string cmd = "INFO_THREAD " + context.getThreadUuid();
    return sendCommand(cmd);
}

bool CommandHandler::execute(const std::string &command, const std::vector<std::string> &args) {
    if (command == "help") { return handleHelp(); }
    if (command == "login") { return handleLogin(args); }
    if (command == "logout") { return handleLogout(); }
    if (command == "users") { return handleUsers(); }
    if (command == "user") { return handleUser(args); }
    if (command == "send") { return handleSend(args); }
    if (command == "messages") { return handleMessages(args); }
    if (command == "subscribe") { return handleSubscribe(args); }
    if (command == "unsubscribe") { return handleUnsubscribe(args); }
    if (command == "subscribed") { return handleSubscribed(args); }
    if (command == "use") { return handleUse(args); }
    if (command == "create") { return handleCreate(args); }
    if (command == "list") { return handleList(); }
    if (command == "info") { return handleInfo(); }

    return false;
}
