#include "Commands.hpp"

void ban(Server *server, int const client_fd, cmd_struct cmd_info) {
    Client &requester = retrieveClient(server, client_fd);
    std::string requesterNickName = requester.getNickName();
    std::string channelName = getChannelName(cmd_info.msg);
    std::string bannedName = getKickedName(cmd_info.msg);
    std::string reason = getReason(cmd_info.msg);

    reason = (reason.empty()) ? ":Has been BANNED by operator." : reason;

    std::map<std::string, Channel> &channels = server->getChannels();
    std::map<std::string, Channel>::iterator it_chan = channels.find(channelName);

    if(channelName.empty() || bannedName.empty()){
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(requesterNickName, cmd_info.name));
        return;
    }
    else if(it_chan == channels.end()) {
        addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(requesterNickName, channelName));
        return;
    }
    else if(!it_chan->second.isChannelClient(requesterNickName)) {
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(requesterNickName, channelName));
        return;
    }
    else if(!it_chan->second.isChannelClient(bannedName)){
        addToClientBuffer(server, client_fd, ERR_USERNOTINCHANNEL(requesterNickName, bannedName, channelName));
        return;
    }
    else if (!it_chan->second.isOperator(requesterNickName)){
        std::cout << "operator " << requesterNickName << std::endl;
        addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(requesterNickName, channelName));
        return;
    }
    else {
        std::map<std::string, Client>::iterator member = it_chan->second.getUsers().begin();
        while (member != it_chan->second.getUsers().end()) {
            addToClientBuffer(server, member->second.getFD(), RPL_BANNED(user_id(requester.getNickName(), requester.getUserName()), it_chan->second.getName(), bannedName, reason));
            member++;
        }
        it_chan->second.getUsers().erase(bannedName);
        it_chan->second.addToBanned(bannedName);
    }
}