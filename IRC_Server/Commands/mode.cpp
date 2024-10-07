#include "Commands.hpp"

static std::string getOp(std::string message) {
    std::string op;
    size_t pos = message.find("+o ");
    op = message.substr(pos + 3, std::string::npos);
    return op;
}

static std::string getROp(std::string message) {
    std::string removed;
    size_t pos = message.find("-o ");
    pos = message.find("-o ");
    removed = message.substr(pos + 3, std::string::npos);
    return removed;
}

static bool promoteToOp(Server *server, int const client_fd, std::map<std::string, Channel>::iterator &channel_it, cmd_struct cmd_info) {
    if (cmd_info.msg.find("+o ") == std::string::npos)
        return false;
    Client &client = retrieveClient(server, client_fd);
    std::string toBeOperator = getOp(cmd_info.msg);
    std::map<std::string, Client>::iterator user = channel_it->second.getUsers().find(client.getNickName());
    if(user == channel_it->second.getUsers().end()){
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client.getNickName(), channel_it->first));
        return true;
    }
    for(std::vector<std::string>::iterator it = channel_it->second.getOperators().begin(); it < channel_it->second.getOperators().end(); it++) {
        if (*it == client.getNickName()) {
            addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client.getNickName(), channel_it->first));
            return true;
        }
    }
    for(std::vector<std::string>::iterator it = channel_it->second.getOperators().begin(); it < channel_it->second.getOperators().end(); it++) {
        if (*it == toBeOperator) {
            addToClientBuffer(server, client_fd, ERR_CLIENTALREDYOP(client.getNickName(), channel_it->first, toBeOperator));
            return true;
        }
    }
    user = channel_it->second.getUsers().find(toBeOperator);
    if(user == channel_it->second.getUsers().end()){
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(toBeOperator, channel_it->first));
        return true;
    }
    channel_it->second.addOperator(toBeOperator);
    addToClientBuffer(server, client_fd, RPL_ADDEDOPERATOR(client.getNickName(), channel_it->first, toBeOperator));
    return true;
}

static bool downGradeOp(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, cmd_struct cmd_info) {
    Client &client = retrieveClient(server, client_fd);
    std::string toBeRemoved = getROp(cmd_info.msg);
    std::map<std::string, Client>::iterator user = channel_it->second.getUsers().find(client.getNickName());
    if(user == channel_it->second.getUsers().end()) {
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client.getNickName(), channel_it->first));
        return true;
    }
    for(std::vector<std::string>::iterator it = channel_it->second.getOperators().begin(); it < channel_it->second.getOperators().end(); it++) {
        if (*it == client.getNickName()) {
            addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client.getNickName(), channel_it->first));
            return true;
        }
    }
    std::vector<std::string>::iterator it = channel_it->second.getOperators().begin();
    while (it != channel_it->second.getOperators().end())
    {
        if(*it == toBeRemoved){
            channel_it->second.removeOperator(toBeRemoved);
            addToClientBuffer(server, client_fd, RPL_REMOVEDOPERATOR(client.getNickName(), channel_it->first, toBeRemoved));
            return true;
        }
        it++;
    }
    addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(toBeRemoved, channel_it->first));
    return true;
}

static std::string getModesToRemove(Server *server, const int client_fd, cmd_struct cmd_info) {
    std::string modesToremove;
    modesToremove.clear();
    while (size_t plusPos = cmd_info.msg.find("-") != std::string::npos) {
        std::string mode = cmd_info.msg.substr(plusPos + 1, cmd_info.msg.find(" "));
        if(mode.empty())
            return mode;
        cmd_info.msg.erase(cmd_info.msg.find(mode), mode.size());
        for(size_t i = 0; i < mode.size(); i++) {
            if(mode.at(i) == 'i' || mode.at(i) == 't' || mode.at(i) == 'k' || mode.at(i) == 'o' || mode.at(i) == 'l') {
                modesToremove += mode.at(i);
                i++;
            }
            else {
                addToClientBuffer(server, client_fd, ERR_BADMODEPARAM(server->getClients().find(client_fd)->second.getNickName(), cmd_info.name));
                modesToremove.clear();
                return modesToremove;
            }
        }
    }
    if (modesToremove.size() != 1){
        addToClientBuffer(server, client_fd, ERR_BADMODEPARAM(server->getClients().find(client_fd)->second.getNickName(), cmd_info.name));
        modesToremove.clear();
        return modesToremove;
    }
    return (modesToremove);
}

static std::string getModesToAdd(Server *server, const int client_fd, cmd_struct cmd_info) {
    std::string modesToAdd;
    modesToAdd.clear();
    while (size_t plusPos = cmd_info.msg.find("+") != std::string::npos) {
        std::string mode = cmd_info.msg.substr(plusPos + 1, cmd_info.msg.find(" "));
        if(mode.empty())
            return mode;
        cmd_info.msg.erase(cmd_info.msg.find(mode), mode.size());
        for(size_t i = 0; i < mode.size(); i++) {
            if(mode.at(i) == 'i' || mode.at(i) == 't' || mode.at(i) == 'k' || mode.at(i) == 'o' || mode.at(i) == 'l') {
                modesToAdd += mode.at(i);
                i++;
            }
            else {
                addToClientBuffer(server, client_fd, ERR_BADMODEPARAM(server->getClients().find(client_fd)->second.getNickName(), cmd_info.name));
                modesToAdd.clear();
                return modesToAdd;
            }
        }
    }
    if (modesToAdd.size() != 1){
        addToClientBuffer(server, client_fd, ERR_BADMODEPARAM(server->getClients().find(client_fd)->second.getNickName(), cmd_info.name));
        modesToAdd.clear();
        return modesToAdd;
    }
    return (modesToAdd);
}

static void chanModed(Server *server, const int client_fd, Channel &channel, std::string mode, std::string modeMsg) {
    //to change the channel mode if added to the channel and remove it if removed
}

static void addModes(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, std::string addedModes, std::string modeMsg) {
    bool add;
    std::string toAdd;
    toAdd.clear();
    for (size_t j = 0; j < addedModes.size(); j++) {
        add = true;
        for (size_t i = 0; i < channel_it->second.getTopic().size(); i++) {
            if (channel_it->second.getTopic().at(i) == addedModes.at(j))
                add = false;
        }
        if(add == true) {
            toAdd += addedModes.at(j);
        }
    }
    if(!toAdd.empty()) {
        channel_it->second.addMode(toAdd);
        chanModed(server, client_fd, channel_it->second, toAdd, modeMsg);
    }
}

static void removeModes(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, std::string removedModes) {
    bool remove;
    std::string toRemove;
    toRemove.clear();
    for (size_t i = 0; i < removedModes.size(); i++) {
        remove = true;
        for (size_t j = 0; i < channel_it->second.getTopic().size(); j++) {
            if (removedModes.at(i) == channel_it->second.getTopic().at(j))
                remove = false;
        }
        if(remove == true)
            toRemove += removedModes.at(i);
    }
    if(!toRemove.empty())
        channel_it->second.removeMode(toRemove);
    
}

static std::string getMessage(std::string str) {
    std::string msg;
    size_t pos = str.rfind("+");
    size_t mpos = str.rfind("-");
    if(pos != std::string::npos || mpos != std::string::npos) {
        msg = str.substr(str.rfind(" "));
        return msg;
    }
    msg.clear();
    return msg;
}

static void changeChannelMode(Server *server, const int client_fd, cmd_struct cmd_info, std::string channelName) {
    std::map<std::string, Channel>::iterator channel_it = server->getChannels().find(channelName);
    std::string addedModes = getModesToAdd(server, client_fd, cmd_info);
    std::string removedModes = getModesToRemove(server, client_fd, cmd_info);
    std::string modeMsg = getMessage(cmd_info.msg);
    if (!addedModes.empty())
        addModes(server, client_fd, channel_it, addedModes, modeMsg);
    if (!removedModes.empty())
        removeModes(server, client_fd, channel_it, removedModes);

}

void mode(Server *server, int const client_fd, cmd_struct cmd_info) {
    std::map<int, Client>::iterator client_it = server->getClients().find(client_fd);

    if(cmd_info.msg.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_it->second.getNickName(), cmd_info.name));
        return;
    }
    size_t chanPos = cmd_info.msg.find("#");
    if(chanPos == std::string::npos) {
        addToClientBuffer(server, client_fd, ERR_NOCHANNELGIVEN(client_it->second.getNickName(), cmd_info.name));
        return;
    }
    std::string channelName = cmd_info.msg.substr(chanPos + 1, cmd_info.msg.find(" "));
    if(channelName.empty()) {
        addToClientBuffer(server, client_fd, ERR_NOCHANNELGIVEN(client_it->second.getNickName(), cmd_info.name));
        return;
    }
    std::map<std::string, Channel>::iterator channel_it = server->getChannels().find(channelName);
    if(channel_it == server->getChannels().end()) {
        addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(client_it->second.getNickName(), channelName));
        return;
    }
    std::vector<std::string>::iterator op = channel_it->second.getOperators().begin();
    while (op != channel_it->second.getOperators().end()) {
        if(*op == client_it->second.getNickName()) {
            if(!promoteToOp(server, client_fd, channel_it, cmd_info) && !downGradeOp(server, client_fd, channel_it, cmd_info))
                changeChannelMode(server, client_fd, cmd_info, channelName);
            return;
        }
        op++;
    }
    addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client_it->second.getNickName(), channelName));
}
