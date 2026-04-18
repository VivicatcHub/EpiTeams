#include "class/LoggingHandlerFactory.hpp"

std::unique_ptr<LoggingHandler> LoggingHandlerFactory::createHandler(const std::string &command, const std::vector<std::string> &args, const Context &context)
{
    if (command == "login") return std::make_unique<LoginHandler>();
    else if (command == "logout") return std::make_unique<LogoutHandler>();
    else if (command == "users") return std::make_unique<UsersHandler>();
    else if (command == "user") return std::make_unique<UserHandler>(args);
    else if (command == "send") return std::make_unique<SendHandler>(args);
    else if (command == "messages") return std::make_unique<MessagesHandler>(args);
    else if (command == "subscribe") return std::make_unique<SubscribeHandler>(args);
    else if (command == "subscribed") return std::make_unique<SubscribedHandler>(args);
    else if (command == "unsubscribe") return std::make_unique<UnsubscribeHandler>(args);
    else if (command == "create_team") return std::make_unique<CreateTeamHandler>(args);
    else if (command == "create_channel") return std::make_unique<CreateChannelHandler>(context);
    else if (command == "create_thread") return std::make_unique<CreateThreadHandler>(context);
    else if (command == "create_comment") return std::make_unique<CreateCommentHandler>(context);
    else if (command == "list_teams") return std::make_unique<ListTeamsHandler>();
    else if (command == "list_channels") return std::make_unique<ListChannelsHandler>(context);
    else if (command == "list_threads") return std::make_unique<ListThreadsHandler>(context);
    else if (command == "list_comments") return std::make_unique<ListCommentsHandler>(context);
    else if (command == "info_user") return std::make_unique<InfoUserHandler>();
    else if (command == "info_team") return std::make_unique<InfoTeamHandler>(context);
    else if (command == "info_channel") return std::make_unique<InfoChannelHandler>(context);
    else if (command == "info_thread") return std::make_unique<InfoThreadHandler>(context);

    return nullptr;
}
