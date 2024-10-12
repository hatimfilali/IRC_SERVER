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
    }
    return true;
}

static bool downGradeOp(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, cmd_struct cmd_info) {
    if (cmd_info.msg.find("-o ") == std::string::npos)
        return false;
    Client &client = retrieveClient(server, client_fd);
    std::string toBeRemoved = getROp(cmd_info.msg);
    std::map<std::string, Client>::iterator user = channel_it->second.getUsers().find(client.getNickName());
    if(user == channel_it->second.getUsers().end()) {
        addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client.getNickName(), channel_it->first));
        return true;
    }
    for(std::vector<std::string>::iterator it = channel_it->second.getOperators().begin(); it < channel_it->second.getOperators().end(); it++) {
        if (*it == client.getNickName()) {
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
    }
    addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client.getNickName(), channel_it->first));
    return true;
}

static std::string getModesToRemove(Server *server, const int client_fd, cmd_struct cmd_info) {
    std::string modesToremove;
    modesToremove.clear();
    size_t minusPos = cmd_info.msg.find("-");
    if (minusPos == cmd_info.msg.npos)
        return modesToremove;
    else
        if( cmd_info.msg.at(minusPos + 1) != ' ' &&  cmd_info.msg.at(minusPos + 1) !=  cmd_info.msg.npos)
            modesToremove = cmd_info.msg.at(minusPos + 1); 
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
    size_t plusPos = cmd_info.msg.find("+");
    if (plusPos == cmd_info.msg.npos)
        return modesToAdd;
    else
        if( cmd_info.msg.at(plusPos + 1) != ' ' &&  cmd_info.msg.at(plusPos + 1) !=  cmd_info.msg.npos)
            modesToAdd = cmd_info.msg.at(plusPos + 1); 
    if (modesToAdd.size() != 1){
        addToClientBuffer(server, client_fd, ERR_BADMODEPARAM(server->getClients().find(client_fd)->second.getNickName(), cmd_info.name));
        modesToAdd.clear();
        return modesToAdd;
    }
    return (modesToAdd);
}

static void chanModed(Server *server, const int client_fd, Channel &channel, std::string mode, std::string modeMsg) {
    if(mode == "+k") {
        if (modeMsg.empty()) {
            addToClientBuffer(server, client_fd, "MODE: Password not presented\r\n");
            return;
        }
        channel.setPassword(modeMsg);
        addToClientBuffer(server, client_fd, "MODE: Password updated\r\n");
    }
    else if (mode == "-k") {
        channel.setPassword("");
        addToClientBuffer(server, client_fd, "MODE: Password removed\r\n");
    }
    if (mode == "+l") {
        if (modeMsg.empty()) {
            addToClientBuffer(server, client_fd, "MODE: limit not presented\r\n");
            return;
        }
        channel.setCapacity(atoi(modeMsg.c_str()));
        addToClientBuffer(server, client_fd, "MODE: limit updated\r\n");
    }
    else if(mode == "-l" ){
        channel.setCapacity(-1);
        addToClientBuffer(server, client_fd, "MODE: limit removed\r\n");
    }
}

static void addModes(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, std::string addedModes, std::string modeMsg) {
    bool add = true;
    std::string toAdd;
    toAdd.clear();
    for (size_t i = 0; i < channel_it->second.getMod().size(); i++) {
        if (channel_it->second.getMod().at(i) == addedModes.at(0))
            add = false;
    }
    if(add == true) {
        toAdd = addedModes.at(0);
    }
    if(!toAdd.empty()) {
        channel_it->second.addMode(toAdd);
        chanModed(server, client_fd, channel_it->second, "+" + toAdd, modeMsg);
    }
}

static void removeModes(Server *server, const int client_fd, std::map<std::string, Channel>::iterator &channel_it, std::string removedModes) {
    bool remove = false;
    std::string toRemove;
    toRemove.clear();
    for (size_t j = 0; j < channel_it->second.getMod().size(); j++) {
        if (removedModes.at(0) == channel_it->second.getMod().at(j))
            remove = true;
    }
    if(remove == true)
        toRemove = removedModes.at(0);
    if(!toRemove.empty()) {
        channel_it->second.removeMode(toRemove);
        chanModed(server, client_fd, channel_it->second, "-" + toRemove, "");
    }
    
}

static std::string getMessage(std::string str) {
    std::string msg;
    size_t pos = str.find("+");
    size_t mpos = str.find("-");
    if(pos != std::string::npos || mpos != std::string::npos) {
        msg = str.substr(str.rfind(" ") + 1);
        return msg;
    }
    return "";
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

void showchannelMode(Server *server,std::string nickname, const int client_fd, Channel &channel) {
    addToClientBuffer(server, client_fd, RPL_CHANNELMODES(nickname, channel.getName(), channel.getMod()));
}

void mode(Server *server, int const client_fd, cmd_struct cmd_info) {
    std::map<int, Client>::iterator client_it = server->getClients().find(client_fd);
    std::string channelName = findChannel(cmd_info.msg);

    if(cmd_info.msg.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_it->second.getNickName(), cmd_info.name));
        return;
    }
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
            if(!promoteToOp(server, client_fd, channel_it, cmd_info) && !downGradeOp(server, client_fd, channel_it, cmd_info)) {
                changeChannelMode(server, client_fd, cmd_info, channelName);
                showchannelMode(server, *op, client_fd, channel_it->second);
            }
            return;
        }
        op++;
    }
    size_t pos = cmd_info.msg.find('#');
    cmd_info.msg.erase(0, pos);
    for(pos = 0; pos < cmd_info.msg.size() - 1; pos++){
        if (cmd_info.msg.at(pos) == ' ')
            break;
    }
    if (op == channel_it->second.getOperators().end() && channel_it->second.getUsers().find(client_it->second.getNickName()) != channel_it->second.getUsers().end() && pos >= cmd_info.msg.size() - 1) {
        showchannelMode(server, client_it->second.getNickName(), client_fd, channel_it->second);
        return;
    }
    addToClientBuffer(server, client_fd, ERR_NOTCHANNELOP(client_it->second.getNickName(), channelName));
}
