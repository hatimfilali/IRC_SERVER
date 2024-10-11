#include "Commands.hpp"

void broadcastToChannel(Server *server, int const client_fd, std::map<int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel, std::string target, std::string msgToSend) {
    std::vector<std::string> kickedUsers = it_channel->second.getKickedUsers();
    for (std::vector<std::string>::iterator it_kicked = kickedUsers.begin(); it_kicked != kickedUsers.end(); it_kicked++)
    {
        if(*it_kicked == it_client->second.getNickName()) {
            std::cout << "[Server] " << *it_kicked << " is kicked from channel and can't send messages no more." << std::endl;
            return;
        }
    }
    std::map<std::string, Client>::iterator member = it_channel->second.getUsers().begin();
    while(member != it_channel->second.getUsers().end()) {
        if(member->second.getFD() != client_fd)
            addToClientBuffer(server, member->second.getFD(), RPL_PRIVMSG(it_client->second.getNickName(), it_client->second.getUserName(), target, msgToSend));
        member++;
    }
    
}


void privmsg(Server *server, int const client_fd, cmd_struct cmd_info) {
    std::map<int, Client> &clients = server->getClients();
    std::map<std::string, Channel> &channels = server->getChannels();
    std::map<int, Client>::iterator it = clients.find(client_fd);
    std::string target;
    std::string msgToSend;

    //parse msg
    // std::cout << "message to parse: " << cmd_info.msg << std::endl;
    size_t pos = cmd_info.msg.rfind(":");
    if (pos == std::string::npos) {
        addToClientBuffer(server, client_fd, ERR_NOTEXTTOSEND(it->second.getNickName()));
        return;
    }
    target = cmd_info.msg.substr(1, pos - 1);//-->set the target
    if (target.empty()) {
        addToClientBuffer(server, client_fd, ERR_NORECIPIENT(it->second.getNickName()));
        return;
    }
    msgToSend = cmd_info.msg.substr(pos + 1); //--> set the msg
    // std::cout << "messageto send: " << msgToSend << std::endl;
    pos = target.find(" "); //--> clean the target
    if (pos != std::string::npos)
        target.erase(pos);
    // std::cout << "target is : " << target << std::endl;
    if (target[0] == '#') { //-->channel case 
        target.erase(0, 1);
        std::map<std::string, Channel>::iterator channelToSend = channels.find(target);
        if (channelToSend == channels.end()) 
            addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it->second.getNickName(), target));
        else
            broadcastToChannel(server, client_fd, it, channelToSend, "#" + target, msgToSend);
    }
    else { //--> no "#" in the beguining
        std::map<std::string, Channel>::iterator chan = channels.find(target);
        std::map<int, Client>::iterator usr = clients.begin();
        while (usr != clients.end()) {
            if (usr->second.getNickName() == target)
                break;
            usr++;
        }
        if(usr == clients.end() && chan == channels.end()) { // -- taarget is no client nor channel
            if (target == "bot") // target is the bot
                //call the boot to respond
                return;
            else // -->target not found
                addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it->second.getNickName(), target));
        }
        else {
            if(usr == clients.end()) { // -->target is channel with no "#" at the beguining
                if(chan->second.isChannelClient(target)) {
                    target.insert(0, "#");
                    broadcastToChannel(server, client_fd, it, chan, target, msgToSend);
                }    
                else
                    addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it->second.getNickName(), target));
            }
            else
            addToClientBuffer(server, usr->first, RPL_PRIVMSG(it->second.getNickName(), it->second.getUserName(), target, msgToSend));
        }
    }

}