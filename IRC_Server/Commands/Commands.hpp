#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "../Macros/Macros.hpp"
#include "../Server.hpp"

class Server;

struct cmd_struct {
    std::string prefix;
    std::string name;
    std::string msg;
};

//PARSING FUNCTIONS
void getCommandLine(Server *server, int const client_fd, std::string cmd_line);
void executeCommand(Server *server, int const client_fd, std::string rcvBuffer);

int parse_cmd(std::string cmd_line, cmd_struct &cmd_info);
void addToClientBuffer(Server *server, int const client_fd, std::string reply);
Client &retrieveClient(Server *server, int const client_fd);
std::string getListOfmembers(std::string client, Channel &channel);
std::string getChannelName(std::string msg);
std::string findNickname(std::string msg);
bool isAlreadyUsed(Server *server, int client_fd, std::string newNickName);
std::string findChannel(std::string msg);
bool ContainAtLeastOneAlphaChar(std::string msg);
void addChannel(Server *server, std::string channelName);
std::string retrieveKey(std::string msg);
bool addClientToChannel(Server *server, std::string &channelName, Client &client);
void sendChannelInfo(Server *server, Channel &channel, std::string channelName, Client &client);
std::string getKickedName (std::string msg);
std::string getReason(std::string msg);


//OPERATOR COMMANDES
void invite(Server *server, int const client_fd, cmd_struct cmd_info);
void join(Server *server, int client_fd, cmd_struct cmd_info);
void  privmsg(Server *server, int const client_fd, cmd_struct cmd_info);
void kick(Server *server, int const client_fd, cmd_struct cmd_info);
void topic(Server *server, int const client_fd, cmd_struct cmd_info);
void mode(Server *server, int const client_fd, cmd_struct cmd_info);
void bot(Server &server, int client_fd, cmd_struct cmd_info);

#endif