#include "class/MessageParser.hpp"
#include <cctype>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

std::string MessageParser::cleanString(const std::string &value)
{
    size_t start = 0;
    size_t end = value.size();

    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) end--;
    return value.substr(start, end - start);
}

std::string MessageParser::extractUuid(const std::string &message)
{
    size_t open_pos = message.find('(');
    size_t close_pos = message.find(')');

    if (open_pos == std::string::npos || close_pos == std::string::npos || open_pos >= close_pos) return "";
    return message.substr(open_pos + 1, close_pos - (open_pos + 1));
}

bool MessageParser::parseUserStatus(const std::string &message, std::string &user_name, std::string &user_uuid, int &user_status)
{
    const std::string open = " (";
    const std::string middle = ") -> ";
    size_t open_pos = message.find(open);
    size_t middle_pos = message.find(middle);
    std::string status;

    if (open_pos == std::string::npos || middle_pos == std::string::npos) return false;
    if (open_pos == 0 || middle_pos <= open_pos + open.size()) return false;

    user_name = message.substr(0, open_pos);
    user_uuid = message.substr(open_pos + open.size(), middle_pos - (open_pos + open.size()));
    status = message.substr(middle_pos + middle.size());

    if (status == "ONLINE") user_status = 1;
    else if (status == "OFFLINE") user_status = 0;
    else return false;

    return !user_name.empty() && !user_uuid.empty();
}

bool MessageParser::parseLoginSuccess(const std::string &message, std::string &user_name, std::string &user_uuid)
{
    const std::string prefix = "Logged in as ";
    const std::string suffix = ".";

    if (message.compare(0, prefix.size(), prefix) != 0) return false;
    if (message.size() <= prefix.size() + suffix.size()) return false;

    size_t open_pos = message.find(" (", prefix.size());
    size_t close_pos = message.find(").", open_pos);

    if (open_pos == std::string::npos || close_pos == std::string::npos || open_pos >= close_pos) return false;

    user_name = message.substr(prefix.size(), open_pos - prefix.size());
    user_uuid = message.substr(open_pos + 2, close_pos - (open_pos + 2));

    return !user_name.empty() && !user_uuid.empty();
}

bool MessageParser::parseLogoutSuccess(const std::string &message, std::string &user_name, std::string &user_uuid)
{
    const std::string prefix = "Logged out ";
    const std::string suffix = ".";

    if (message.compare(0, prefix.size(), prefix) != 0) return false;
    if (message.size() <= prefix.size() + suffix.size()) return false;

    size_t open_pos = message.find(" (", prefix.size());
    size_t close_pos = message.find(").", open_pos);

    if (open_pos == std::string::npos || close_pos == std::string::npos || open_pos >= close_pos) return false;

    user_name = message.substr(prefix.size(), open_pos - prefix.size());
    user_uuid = message.substr(open_pos + 2, close_pos - (open_pos + 2));

    return !user_name.empty() && !user_uuid.empty();
}

bool MessageParser::parseTeamInfo(const std::string &message, std::string &team_name, std::string &team_uuid, std::string &team_description)
{
    const std::string open = " (";
    const std::string middle = ") - ";
    size_t open_pos = message.find(open);
    size_t middle_pos = message.find(middle);

    if (open_pos == std::string::npos || middle_pos == std::string::npos) return false;
    if (open_pos == 0 || middle_pos <= open_pos + open.size()) return false;

    team_name = message.substr(0, open_pos);
    team_uuid = message.substr(open_pos + open.size(), middle_pos - (open_pos + open.size()));
    team_description = message.substr(middle_pos + middle.size());

    return !team_name.empty() && !team_uuid.empty();
}

bool MessageParser::parseChannelInfo(const std::string &message, std::string &channel_name, std::string &channel_uuid, std::string &channel_description)
{
    const std::string open = " (";
    const std::string middle = ") - ";
    size_t open_pos = message.find(open);
    size_t middle_pos = message.find(middle);

    if (open_pos == std::string::npos || middle_pos == std::string::npos) return false;
    if (open_pos == 0 || middle_pos <= open_pos + open.size()) return false;

    channel_name = message.substr(0, open_pos);
    channel_uuid = message.substr(open_pos + open.size(), middle_pos - (open_pos + open.size()));
    channel_description = message.substr(middle_pos + middle.size());

    return !channel_name.empty() && !channel_uuid.empty();
}

bool MessageParser::parseThreadInfo(const std::string &message, std::string &thread_uuid, std::string &user_uuid, long &timestamp, std::string &thread_title, std::string &thread_body)
{
    std::istringstream iss(message);
    std::string token, title_start;
    size_t first_quote, second_quote, third_quote, fourth_quote;

    // Extract first three tokens (thread_uuid, user_uuid, timestamp)
    if (!(iss >> thread_uuid >> user_uuid >> timestamp)) return false;

    // Find the quoted strings for title and body
    first_quote = message.find('"');
    if (first_quote == std::string::npos) return false;

    second_quote = message.find('"', first_quote + 1);
    if (second_quote == std::string::npos) return false;

    third_quote = message.find('"', second_quote + 1);
    if (third_quote == std::string::npos) return false;

    fourth_quote = message.find('"', third_quote + 1);
    if (fourth_quote == std::string::npos) return false;

    thread_title = message.substr(first_quote + 1, second_quote - (first_quote + 1));
    thread_body = message.substr(third_quote + 1, fourth_quote - (third_quote + 1));

    return !thread_uuid.empty() && !user_uuid.empty();
}

bool MessageParser::parsePrivateMessage(const std::string &message, std::string &sender_uuid, long &timestamp, std::string &message_body)
{
    // <send_uuid>: <message_body> (<timestamp>)
    size_t colon_pos = message.find(':');
    size_t open_pos = message.find('(', colon_pos);
    size_t close_pos = message.find(')', open_pos);

    if (colon_pos == std::string::npos || open_pos == std::string::npos || close_pos == std::string::npos) return false;
    if (colon_pos == 0 || open_pos <= colon_pos + 1 || close_pos <= open_pos + 1) return false;

    sender_uuid = message.substr(0, colon_pos);
    message_body = message.substr(colon_pos + 1, open_pos - (colon_pos + 1));
    std::string timestamp_str = message.substr(open_pos + 1, close_pos - (open_pos + 1));
    try { timestamp = std::stol(timestamp_str); }
    catch (const std::exception &) { return false; }
    return !sender_uuid.empty() && !message_body.empty();
}

bool MessageParser::parseMessageReceipt(const std::string &message, std::string &msg, std::string &from)
{
    // New message from <from> : <message>"
    const std::string prefix = "New message from ";
    const std::string middle = " : ";
    size_t prefix_pos = message.find(prefix);
    size_t middle_pos = message.find(middle);

    if (prefix_pos == std::string::npos || middle_pos == std::string::npos) return false;
    if (prefix_pos + prefix.size() >= middle_pos) return false;

    from = message.substr(prefix_pos + prefix.size(), middle_pos - (prefix_pos + prefix.size()));
    msg = message.substr(middle_pos + middle.size());
    return !from.empty() && !msg.empty();
}

void MessageParser::printResponsePrompt(const std::string &code)
{
    const char *env = std::getenv("ENV");
    bool is_dev = (env && std::string(env) == "dev");
    bool interactive_mode = isatty(STDIN_FILENO);

    if (interactive_mode) std::cout << "  ╰─> ";
    if (is_dev) std::cout << "[" << code << "]: ";
    std::cout.flush();
}

void MessageParser::printResponseFailure(const std::string &message)
{
    bool interactive_mode = isatty(STDIN_FILENO);

    std::cout << "Fail to parse response ─> ";
    if (interactive_mode) std::cout << "  ╰─> ";
    if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "[500]: ";
    std::cout << message << std::endl;
}
