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
            std::cout << "No channel name" << std::endl;
            return;
        } 
        cmd_info.msg.erase(cmd_info.msg.find(channelName), channelName.length());

        std::map<std::string, Channel> &ChannelList = server->getChannels();
        std::map<std::string, Channel>::iterator it = ChannelList.find(channelName);
        for (std::vector<std::string>::iterator oper = it->second.getOperators().begin(); oper < it->second.getOperators().begin(); oper++)
        {
            std::cout << "operator: " << *oper << std::endl;
        }
        if (it == ChannelList.end()) {
            addChannel(server, channelName);
            std::cout << clientNickName << "<----------" << std::endl;
            it = ChannelList.find(channelName);
            it->second.addFirstOperator(clientNickName);
            std::cout << " first operator added to channel: @"<< client.getNickName() << " #" << channelName << "\n";
            // std::cout<< "channel added: " << channelName << "\n";
        }
        // else if(it->second.getMod().find("k") != std::string::npos) {
        //     std::string key = retrieveKey(cmd_info.msg);
        //     cmd_info.msg.erase(cmd_info.msg.find(key), key.length());
        //     if (it->second.getPassword() != key) {
        //         addToClientBuffer(server, client_fd, ERR_BADCHAANNELKEY(clientNickName, channelName));
        //         continue;
        //     }
        // }
            std::map<std::string, Channel>::iterator iter = ChannelList.find(channelName);
            it = ChannelList.find(channelName);
            // std::cout << "channel found : " << iter->second.getName()<< "\n";
            // if  (iter->second.getCapacity() != -1 && (int)iter->second.getUsers().size() != iter->second.getCapacity()) {
            //     addToClientBuffer(server, client_fd, ERR_CHANNELISFULL(clientNickName, channelName));
            //     continue;
            // }
            if (it->second.isBanned(clientNickName) == true) 
                addToClientBuffer(server, client_fd, ERR_BANNEDFROMCHANNEL(clientNickName, channelName));
            else {
                if(addClientToChannel(server, channelName, client) == true) { 
                    sendChannelInfo(server, it->second, channelName, client);
                    
                    // std::cout << "hna mzyan"<<std::endl;
                }
            }
    }
    
}