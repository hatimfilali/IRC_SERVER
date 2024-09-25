#include "Commands.hpp"

void invite(Server *server, int const client_fd, cmd_struct cmd_info) {
    Client &client = retrieveClient(server, client_fd);
    std::string clientNickName = client.getNickName();
    std::string invitedClient = findNickname(cmd_info.msg);
    std::string channelName = findChannel(cmd_info.msg);

    if (clientNickName.empty() || channelName.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(clientNickName, cmd_info.name));
        return;
    }
    //check if channel exist
    std::map<std::string, Channel> &channels = server->getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(clientNickName, channelName));
        return;
    }
    //check if the invting user is on channel
    if (it->second.isChannelClient(clientNickName) == false) {
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(clientNickName, channelName));
        return;
    }
    //check if the invited user is not on channel already
    if (it->second.isChannelClient(invitedClient) == true) {
        addToClientBuffer(server, client_fd, ERR_USERONCHANNEL(clientNickName, invitedClient, channelName));
        return;
    }
    //check if channel is invite only 
    //...


    addToClientBuffer(server, client_fd, RPL_INVITING(user_id(clientNickName, client.getUserName()), clientNickName, invitedClient, channelName));
    std::map<int, Client> clients = server->getClients();
    std::map<int, Client>::iterator iter = clients.begin();

    while (iter != clients.end()) {
        if (iter->second.getNickName() == invitedClient) {
            addToClientBuffer(server, iter->second.getFD(), RPL_INVITE(user_id(clientNickName, client.getUserName()), invitedClient, channelName));
            break;
        }   
    }
}