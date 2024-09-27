#include "Commands.hpp"
#include "../Macros/Macros.hpp"

//PARSING FUNCTIONS

int parse_cmd(std::string cmd_line, cmd_struct &cmd_info) {
    if (cmd_line.empty())
        return FAILURE;
    
    //Name
    std::string copy = cmd_line;
    if (cmd_line[0] == ':') {
        if(cmd_line.find_first_of(' ') != std::string::npos)
            copy.erase(0, copy.find_first_of(' ') + 1);
    }
    
    //prefix
    if(copy.find_first_of(' ') == std::string::npos) {
        cmd_info.name.insert(0, copy, 0, std::string::npos);
        if(cmd_info.name.find_first_of('\r') != std::string::npos)
            cmd_info.name.erase(cmd_info.name.find('\r'), 1);

    }
    else
        cmd_info.name.insert(0, copy, 0, copy.find_first_of(' '));

    size_t prefix_len = cmd_line.find(cmd_info.name, 0);
    cmd_info.prefix.assign(cmd_line, 0, prefix_len);
    
    //Message
    size_t msg_begin = cmd_line.find_first_of(cmd_info.name, 0) + cmd_info.name.length();
    cmd_info.msg = cmd_line.substr(msg_begin, std::string::npos);
    cmd_info.msg.erase(cmd_info.msg.find('\r'), 1);

    for (size_t i = 0; i < cmd_info.name.size(); i++) {
        cmd_info.name[i] = std::toupper(cmd_info.name[i]);
    }

    return SUCSSES;
}


void addToClientBuffer(Server *server, int const client_fd, std::string reply) {
    Client &client = retrieveClient(server, client_fd);
    client.setSendBuffer(reply);
}

Client &retrieveClient(Server *server, int const client_fd) {
    std::map<int, Client> & clientList = server->getClients();
    std::map<int, Client>::iterator it = clientList.find(client_fd);

    Client &client = it->second;
    return client;
}


std::string getListOfmembers(std::string client, Channel &channel) {
    std::map<std::string, Client> client_list = channel.getUsers();
    std::map<std::string, Client>::iterator it;
    std::string nickName;
    std::string listOfMembers;

    for (it = client_list.begin(); it != client_list.end(); it++) {
        nickName.clear();
        nickName = it->second.getNickName();
        if (channel.isChannelClient(client) == false)
            continue;
        if (channel.isOperator(nickName) == true)
            listOfMembers += "@";
        listOfMembers += nickName;
        listOfMembers += " ";
    }
    if (listOfMembers.size() >= 1 && listOfMembers.at(listOfMembers.size() - 1) == ' ')
        listOfMembers.erase(listOfMembers.end() - 1);
    return listOfMembers;
}

std::string getChannelName(std::string msg) {
    std::string channelName;
    size_t i = 0;
    while (msg[i] &&  !isalpha(msg[i]) && !isdigit(msg[i] && msg[i] != '-' && msg[i] != '_'))
        i++;
    while(msg[i] && (isdigit(msg[i]) ||  isdigit(msg[i]) || msg[i] == '-' || msg[i] == '_'))
        channelName += msg[i];

    return channelName;
}

std::string findNickname(std::string msg) {
    std::string nickName;
    nickName.clear();
    if (msg.empty() == true)
        return nickName;
    char *str = const_cast<char *>(msg.data());
    nickName = strtok(str, " ");
    if(nickName.empty() || nickName.find("#") != nickName.npos)
        nickName.clear();
    return nickName;
}

bool isAlreadyUsed(Server *server, int client_fd, std::string newNickName) {
    std::map<int , Client> clientList = server->getClients();
    std::map<int, Client>::iterator it = clientList.begin();
    while (it != clientList.end()) {
        if (it->second.getNickName() == newNickName)
            return true;
        it++;
    }
    return false;
}

std::string findChannel(std::string msg) { // msg example : INVITE timu #channelName
    std::string ChannelName;
    if (msg.empty() || msg.find("#") == msg.npos)
        ChannelName.clear();
    else
        ChannelName.append(msg, msg.find("#") + 1, msg.npos);
    return ChannelName;
}


bool ContainAtLeastOneAlphaChar(std::string msg) {
    if (msg[0] == ' ')
        msg.erase(0, 1);
    if (msg.find(" ") != msg.npos)
        msg = msg.substr(0, msg.find(" "));
    for (size_t i = 0; i < msg.size(); i++){
        if (isalpha(msg[i]))
            return true;
    }
    return false;
}

void addChannel(Server *server, std::string channelName) {
    std::map<std::string, Channel> channelList = server->getChannels();
    std::map<std::string, Channel>::iterator it = channelList.find(channelName);
    if(it != channelList.end()) {
        std::cout << "Channel name already exists, Please enter another name";
        return;
    }
    Channel channel(channelName);
    server->getChannels().insert(std::pair<std::string, Channel>(channelName, channel));
}

std::string retrieveKey(std::string msg) {
    std::string key;
    key.clear();
    msg = msg.substr(msg.find_last_of(" "), msg.npos);
    
    if (msg[0] == ' ')
        msg.erase(0, 1);
    int pos = msg.find(",") == 0 ? msg.find(",") + 1 : 0;
    while(msg[pos] != ',' || msg[pos]) {
        if (msg[pos] == '-' || msg[pos] == '_' || isalpha(msg[pos]) || isdigit(msg[pos]))
            key += msg[pos];
            pos++;
    }
    return key;
}

void addClientToChannel(Server *server, std::string &channelName, Client &client) {
    std::string clientName = client.getNickName();
    std::map<std::string, Channel>::iterator it = server->getChannels().find(channelName);
    if (it->second.isChannelClient(clientName) == false) {
        it->second.addClient(client);
    }
    else 
        std::cout << "Client already exist\n";
}

void sendChannelInfo(Server *server, Channel &channel, std::string channelName, Client &client) {
    std::string clientNickName = client.getNickName();
    std::string clientUserName = client.getUserName();
    std::map<std::string, Client>::iterator member = channel.getUsers().begin();

    while(member != channel.getUsers().end()) {
        addToClientBuffer(server, member->second.getFD(), RPL_JOIN(user_id(clientNickName, clientUserName), channelName));
        if(channel.getTopic().empty() == true)
            addToClientBuffer(server, member->second.getFD(), RPL_TOPIC(clientNickName, channelName, channel.getTopic()));
        std::string listOfMembers = getListOfmembers(clientNickName, channel);
        addToClientBuffer(server, client.getFD(), RPL_NAMEREPLY(clientNickName, channelName, listOfMembers));
        addToClientBuffer(server, client.getFD(), RPL_ENDOFNAMES(clientNickName, channelName));
        member++;
    }
}

std::string getKickedName(std::string msg) {
    std::string kickedName;

    if(msg[0] == ' ')
        msg.erase(0, 1);
    kickedName  = msg.substr(msg.find(' ') + 1, (msg.find(':') - 1 - msg.find(' ') + 1));
    return kickedName;
}

std::string getReason(std::string msg) {
    std::string reason;
    
    reason.clear();
    if (msg.find(":") != std::string::npos)
        reason.append(msg, msg.find(":" + 1, std::string::npos));
    return reason;
}

static void splitMsg(std::vector<std::string> &cmds, std::string cmd_line) {
    int pos = 0;
    std::string del = "\n";
    std::string substr;

    while ((pos = cmd_line.find(del)) != static_cast<int>(std::string::npos)) {
        substr = cmd_line.substr(0, pos);
        cmds.push_back(substr);
        cmd_line.erase(0, pos + 1);
    }
}

void executeCommand(Server *server, int const client_fd, std::string rcvBuffer) {
    cmd_struct cmd_info;
    Client client = retrieveClient(server, client_fd);
    std::string validCommands[VALID_LENGTH] = {"INVITE", "JOIN", "TOPIC", "PRIVMSG", "KICK", "BOT"};

    if (parse_cmd(rcvBuffer, cmd_info) == FAILURE) 
        return;
        
    size_t i = 0;
    while(i < VALID_LENGTH) {
        if (validCommands[i] == cmd_info.name)
            break;
        i++;
    }
    switch (i + 1)
    {
    case 1:
        invite(server,client_fd, cmd_info);
        break;
    case 2:
        join(server, client_fd, cmd_info);
        break;
    case 3:
        //topic command
        break;
    case 4:
        privmsg(server, client_fd, cmd_info);
    case 5:
        kick(server, client_fd, cmd_info);
        break;
    case 6: // OMAR_BOT
        bot(server, client_fd, cmd_info.msg);
    default:
        addToClientBuffer(server, client_fd, ERR_UNKNONKCOMMAND(client.getNickName(), cmd_info.name));
        break;
    }
}

void getCommandLine(Server *server, int const client_fd, std::string cmd_line) {
    std::vector<std::string> cmds;
    std::map<int, Client>::iterator it = server->getClients().find(client_fd);

    splitMsg(cmds, cmd_line);
    
    for (size_t i = 0; i < cmds.size(); i++)
    {
        executeCommand(server, client_fd, cmds[i]);
    }
    
}