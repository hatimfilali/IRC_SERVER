#include "Commands.hpp"

void kick(Server *server, int const client_fd, cmd_struct cmd_info) {
    Client& requester = retrieveClient(server, client_fd);
    std::string requesterNickName = requester.getNickName();
    std::string channelName = getChannelName(cmd_info.msg);
    std::string kickedName = getKickedName(cmd_info.msg);
    std::string reason = getReason(cmd_info.msg);

    std::map<std::string, Channel> channels = server->getChannels();
    std::map<std::string, Channel>::iterator it_chan = channels.find(channelName);
    reason = (reason.empty()) ? ":Has been kickedd by operator." : reason;

    if(channelName.empty() || kickedName.empty()){
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
    else if(!it_chan->second.isChannelClient(kickedName)){
        addToClientBuffer(server, client_fd, ERR_USERNOTINCHANNEL(requesterNickName, kickedName, channelName));
        return;
    }
    else if (it_chan->second.isOperator(requesterNickName)){
        addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(requesterNickName, channelName));
        return;
    }
    else {
        std::map<std::string, Client>::iterator member = it_chan->second.getUsers().begin();
        while (member != it_chan->second.getUsers().end()) {
            addToClientBuffer(server, member->second.getFD(), RPL_KICK(user_id(requester.getNickName(), requester.getUserName()), it_chan->second.getName(), kickedName, reason));
            member++;
        }
        it_chan->second.getUsers().erase(kickedName);
        it_chan->second.addToKicked(kickedName);
    }
}
