#include "Commands.hpp"

//in macros
#define ERR_NOCHANNELGIVEN(client, command) (client + " " + command + " :No channel given\r\n")
#define RPL_CHANNELHASNOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set.\r\n")

static void showChannelTopic(Server *server, int const client_fd, cmd_struct cmd_info, std::string channelName) {
    std::map<int, Client>::iterator client_it = server->getClients().find(client_fd);
    std::map<std::string, Channel>::iterator channel_it  = server->getChannels().find(channelName);

    std::string channelTopic = channel_it->second.getTopic();
    if(channelTopic.empty()) {
        addToClientBuffer(server, client_fd, RPL_CHANNELHASNOTOPIC(client_it->second.getNickName(), channel_it->first));
        return;
    }
    addToClientBuffer(server, client_fd, RPL_TOPIC(client_it->second.getNickName(), channel_it->first, channelTopic));
}

static void changeChannelTopic(Server *server, const int client_fd, cmd_struct cmd_info, std::string channelName) { // --->this command should change or view the topic depending on the paarams of the command
    std::map<int, Client>::iterator client_it = server->getClients().find(client_fd);;
    std::map<std::string, Channel>::iterator channel_it = server->getChannels().find(channelName);

    size_t pos = cmd_info.msg.find(":");
    if(pos == std::string::npos){//----->if ther is no topic (no ":" in the message)
        showChannelTopic(server, client_fd, cmd_info, channelName);
    }
    std::string param = cmd_info.msg.substr(pos + 1, std::string::npos);
    if (param.empty()) {//---->unset channel topic
        channel_it->second.setTopic(param);
        addToClientBuffer(server, client_fd, RPL_TOPIC(client_it->second.getNickName(), channel_it->first, channel_it->second.getTopic()));
        return;
    }
    channel_it->second.setTopic(param);//->change of the topic
    addToClientBuffer(server, client_fd, RPL_TOPIC(client_it->second.getNickName(), channelName, channel_it->second.getTopic()));
}

void topic(Server *server, int const client_fd, cmd_struct cmd_info) {
    std::map<int, Client>::iterator client_it = server->getClients().find(client_fd);
    if(cmd_info.msg.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_it->second.getNickName(), cmd_info.name));
        return;
    }
    size_t pos = cmd_info.msg.find("#");
    if(pos == std::string::npos) {
        addToClientBuffer(server, client_fd, ERR_NOCHANNELGIVEN(client_it->second.getNickName(), cmd_info.name));
        return;
    }
    std::string channelName = cmd_info.msg.substr(pos + 1, std::string::npos);
    std::map<std::string, Channel>::iterator channel_it = server->getChannels().find(channelName);
    if (channel_it == server->getChannels().end()) {
        addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(client_it->second.getNickName(), channel_it->first));
        return;
    }
    if (channel_it->second.getUsers().find(client_it->second.getNickName()) == channel_it->second.getUsers().end()) {//--->if client not on channel
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client_it->second.getNickName(), channel_it->first));
        return;
    }
    //check if +t exist in channel mode
    if (channel_it->second.getMod().find("t") != std::string::npos) { //----->if the topic command restricted for channel operators only
        std::vector<std::string>::iterator op = channel_it->second.getOperators().begin();
        while(op != channel_it->second.getOperators().end()) {//---->find operator
            if (*op == client_it->second.getNickName()) {
                changeChannelTopic(server, client_fd, cmd_info, channel_it->first); 
                return;
            }
            op++;
        }
        if(cmd_info.msg.find(":") != std::string::npos) {
            addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client_it->second.getNickName(), channel_it->first));
            return;
        }
        showChannelTopic(server, client_fd, cmd_info, channel_it->first);//if client on channel show the topic for the channel
    }
    else {
        changeChannelTopic(server, client_fd, cmd_info, channelName);       
    }
}