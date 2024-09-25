#include "Commands.hpp"
#include "../Macros/Macros.hpp"

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
