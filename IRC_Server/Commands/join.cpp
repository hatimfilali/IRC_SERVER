#include "Commands.hpp"

void join(Server *server, int client_fd, cmd_struct cmd_info) {
    Client client = retrieveClient(server, client_fd);
    std::string clientNickName = client.getNickName();
    std::string channelName;
    if (ContainAtLeastOneAlphaChar(cmd_info.msg) == false) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(clientNickName, cmd_info.name));
        return;
    }
    while(ContainAtLeastOneAlphaChar(cmd_info.msg) == true) {
        channelName.clear();
        channelName = findChannel(cmd_info.msg);

        cmd_info.msg.erase(cmd_info.msg.find(channelName), channelName.length());

        std::map<std::string, Channel> ChannelList = server->getChannels();
        std::map<std::string, Channel>::iterator it = ChannelList.find(channelName);
        if (it == ChannelList.end())
            addChannel(server, channelName);
        else if(it->second.getMod().find("k") != std::string::npos) {
            std::string key = retrieveKey(cmd_info.msg);
            cmd_info.msg.erase(cmd_info.msg.find(key), key.length());
            if (it->second.getPassword() != key) {
                addToClientBuffer(server, client_fd, ERR_BADCHAANNELKEY(clientNickName, channelName));
                continue;
            }
        }
        std::map<std::string, Channel>::iterator iter = ChannelList.find(channelName);
        if  (iter->second.getCapacity() != -1 && (int)iter->second.getUsers().size() != iter->second.getCapacity()) {
            addToClientBuffer(server, client_fd, ERR_CHANNELISFULL(clientNickName, channelName));
            continue;
        }
        if (it->second.isBanned(clientNickName) == true) 
            addToClientBuffer(server, client_fd, ERR_BANNEDFROMCHANNEL(clientNickName, channelName));
        else {
            addClientToChannel(server, channelName, client);
            if(it->second.getOperators().empty())
                it->second.addOperator(clientNickName);
            sendChannelInfo(server, it->second, channelName, client);
        }
    }
}