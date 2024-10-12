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
    cmd_info.msg = cmd_line.substr(msg_begin);
    if (cmd_info.msg.find("\r") != cmd_info.msg.npos)
        cmd_info.msg.erase(cmd_info.msg.find('\r'), 1);

    for (size_t i = 0; i < cmd_info.name.size(); i++) {
        cmd_info.name[i] = std::toupper(cmd_info.name[i]);
    }

    return SUCSSES;
}

//reply

void addToClientBuffer(Server *server, int const client_fd, std::string reply) {
    std::map<int, Client> & clientList = server->getClients();
    std::map<int, Client>::iterator it = clientList.find(client_fd);
    if (it == clientList.end())
        return;

    it->second.setSendBuffer(reply);
    // client.setsendReady(true);
    server->SendJoin(client_fd);
}

Client &retrieveClient(Server *server, int const client_fd) {
    std::map<int, Client> & clientList = server->getClients();
    std::map<int, Client>::iterator it = clientList.find(client_fd);
    // std::cout << "fd : "<< it->first << std::endl << "nickname is :" << it->second.getNickName() << std::endl;
    // if(it == clientList.end())
    // {

    //     std::cout << "client not in the client list\n";
    // }
    // std::cout << "client retireved succefully :"<< it->second.getNickName() << "  \n";
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
    channelName.clear();
    size_t pos = msg.find("#");
    if (pos == std::string::npos)
        return channelName;
    msg.erase(0, pos + 1);
    pos = msg.find(" ");
    channelName = msg.substr(0, pos);
    
    return channelName;
}

std::string findNickname(std::string msg) {
    std::string nickName;
    nickName.clear();
    if (msg.empty() == true)
        return nickName;
   size_t pos = msg.rfind(" ");
   nickName = msg.substr(pos + 1);
    return nickName;
}

bool isAlreadyUsed(Server *server, int client_fd, std::string newNickName) {
    (void) client_fd;
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
    if (msg.empty() || msg.find("#") == msg.npos) {
        ChannelName.clear();
        }
    else{
        size_t pos = msg.find("#");
        msg = msg.substr(pos);
        pos = (msg.find(" ") != msg.npos) ? msg.find(" ") - 1 : msg.npos;
        ChannelName = msg.substr(1, pos);
    }
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
    std::map<std::string, Channel> &channelList = server->getChannels();
    std::map<std::string, Channel>::iterator it = channelList.find(channelName);
    if(it != channelList.end())
        return;
    Channel channel(channelName);
    server->getChannels().insert(std::pair<std::string, Channel>(channelName, channel));
    // std::cout <<"channel added: "<< server->getChannels().find(channelName)->second.getName() << "\n";
}

std::string retrieveKey(std::string msg) {
    std::string key;
    key.clear();
    size_t pos = msg.rfind(" ");
    if (msg.at(pos + 1) == '#')
        return key;
    key = msg.substr(pos + 1);
    return key;
}

bool addClientToChannel(Server *server, std::string &channelName, Client &client) {
    std::string clientName = client.getNickName();
    std::map<std::string, Channel>::iterator it = server->getChannels().find(channelName);
    if (it->second.isChannelClient(clientName) == false) {
        it->second.addClient(client);
        // std::cout << "client added to channel: "<< it->second.getUsers().find(clientName)->second.getFD()<<" | "<< clientName << " #" << channelName << "\n";
        // it->second.printClient();
        return true;
    }
    std::cout << "Client already exist\n";
    return false;
}

void sendChannelInfo(Server *server, Channel &channel, std::string channelName, Client &client) {
    std::string clientNickName = client.getNickName();

    std::map<std::string, Client>::iterator member =channel.getUsers().begin();
    while(member != channel.getUsers().end()) {
        // std::cout << "client fd:" << member->second.getFD()  << "  " <<  RPL_JOIN(clientNickName, channelName) << std::endl;
        // std::cout << "Server fd: " << server->getFd() << std::endl;
        addToClientBuffer(server, member->second.getFD(), RPL_JOIN(clientNickName, channelName));
        addToClientBuffer(server, member->second.getFD(), RPL_TOPIC(member->second.getNickName(), channel.getName(), channel.getTopic()));
        // std::cout << "USERS: " << RPL_NAMEREPLY(member->second.getNickName(), channel.getName(), getListOfmembers(member->second.getNickName(), channel)) << std::endl;
        addToClientBuffer(server, member->second.getFD(), RPL_NAMEREPLY(member->second.getNickName(), channel.getName(), getListOfmembers(member->second.getNickName(), channel)));
        addToClientBuffer(server, member->second.getFD(), RPL_ENDOFNAMES(member->second.getNickName(), channel.getName()));
        if(member->second.getNickName() == clientNickName)
            addToClientBuffer(server, member->second.getFD(), RPL_CHANNELMODES(member->second.getNickName(),channel.getName(), channel.getMod()));   
    member++;
    }
}

std::string getKickedName(std::string msg) {
    std::string kickedName;
    kickedName.clear();
    size_t pos = msg.find(getChannelName(msg));
    pos += getChannelName(msg).length();
    while(msg.at(pos) == ' ')
        pos++;
    msg.erase(0, pos);
    kickedName = msg.substr(0, msg.find(" "));
    return kickedName;
}

std::string getReason(std::string msg) {
    std::string reason;
    reason.clear();
    size_t pos = msg.find(getKickedName(msg));
    while(msg.at(pos) != ' ' && pos < msg.length()-1)
        pos++;
    if(msg.at(pos) != ' ')
        return reason;
    while(msg.at(pos) == ' ' && pos < msg.length()-1)
        pos++;
    if (msg.at(pos) != ' ')
        reason = msg.substr(pos);
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
    // std::cout <<" ReadBuffer is "<< cmd_info.name << std::endl;
    std::string validCommands[VALID_LENGTH] = {"INVITE", "JOIN", "TOPIC", "PRIVMSG", "KICK", "MODE", "BOT"};
    // std::cout << "recieved : " << rcvBuffer << " from: " << client_fd << std::endl;
    if (parse_cmd(rcvBuffer, cmd_info) == FAILURE) 
        return;
    // std::cout << "recieved from client > " << cmd_info.prefix << " " << cmd_info.name << " " << cmd_info.msg << std::endl;
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
        topic(server, client_fd, cmd_info);
        break;
    case 4:
        privmsg(server, client_fd, cmd_info);
        break;
    case 5:
        kick(server, client_fd, cmd_info);
        break;
    case 6:
        mode(server, client_fd, cmd_info);
        break;
    case 7:
        bot(*server, client_fd, cmd_info);
        break;
    default:
        addToClientBuffer(server, client_fd, ERR_UNKNONKCOMMAND(client.getNickName(), cmd_info.name));
        break;
    }
}

void getCommandLine(Server *server, int const client_fd, std::string cmd_line){
    std::vector<std::string> cmds;
    // std::map<int, Client>::iterator it = server->getClients().find(client_fd);

    splitMsg(cmds, cmd_line);
    
    for (size_t i = 0; i < cmds.size(); i++)
    {
        
        executeCommand(server, client_fd, cmds[i]);
    }
    Client &client = retrieveClient(server, client_fd);
    client.setReadReady(false);
    client.setReadBuffer("");
}