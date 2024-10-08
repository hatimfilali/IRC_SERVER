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
        if(channelName.empty()) {
            addToClientBuffer(server, client_fd, ERR_NOCHANNELGIVEN(client.getNickName(), cmd_info.name));
            return;
        } 
        cmd_info.msg.erase(cmd_info.msg.find(channelName), channelName.length());

        std::map<std::string, Channel> &ChannelList = server->getChannels();
        std::map<std::string, Channel>::iterator it = ChannelList.find(channelName);

        if (it == ChannelList.end()) {
            addChannel(server, channelName);
            it = ChannelList.find(channelName);
            it->second.addFirstOperator(clientNickName);
        }
        else {
            if (it->second.getCapacity() != 1 && it->second.getCapacity() == it->second.getUsers().size()){
                addToClientBuffer(server, client_fd, ERR_CHANNELISFULL(client.getNickName(), it->first));
                continue;
            }
            if(it->second.getMod().find("k") != std::string::npos || it->second.getMod().find("i") != std::string::npos) {
                if (it->second.getMod().find("i") != std::string::npos)
                    continue;
                std::string key = retrieveKey(cmd_info.msg);
                cmd_info.msg.erase(cmd_info.msg.find(key), key.length());
                if (it->second.getPassword() != key) {
                    addToClientBuffer(server, client_fd, ERR_BADCHAANNELKEY(clientNickName, channelName));
                    continue;
                }
            }
            it = ChannelList.find(channelName);
            if  (it->second.getCapacity() != -1 && (int)it->second.getUsers().size() != it->second.getCapacity()) {
                addToClientBuffer(server, client_fd, ERR_CHANNELISFULL(clientNickName, channelName));
                continue;
            }
            if(addClientToChannel(server, channelName, client) == true)  
                sendChannelInfo(server, it->second, channelName, client);
        }
    }
}