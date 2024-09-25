#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "../Server.hpp"

class Server;

struct cmd_struct {
    std::string prefix;
    std::string name;
    std::string msg;
};

int parse_cmd(std::string cmd_line, cmd_struct &cmd_info);
void addToClientBuffer(Server *server, int const client_fd, std::string reply);
Client &retrieveClient(Server *server, int const client_fd);
std::string getListOfmembers(std::string client, Channel &channel);
std::string getChannelName(std::string msg);

#endif