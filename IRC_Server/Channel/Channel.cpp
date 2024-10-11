#include "Channel.hpp"

Channel::Channel(std::string const &name) : ChannelName(name), capacity(-1) {
    users.clear();
    topic.clear();
    kickedUsers.clear();
    operators.clear();
}

Channel::~Channel () {}

//Getters &Setters

std::string  &Channel::getName() {return ChannelName;}

std::string  &Channel::getTopic() {return topic;}

std::string  &Channel::getMod() {return mod;}

std::string  &Channel::getPassword() {return password;}

std::string  &Channel::getOperatorPassword() {return operatorPassword;}

int &Channel::getCapacity() {return capacity;}

std::map<std::string, Client> &Channel::getUsers() {return users;}

std::vector<std::string> &Channel::getKickedUsers() {return kickedUsers;}

std::vector<std::string> &Channel::getOperators() {return operators;}

void Channel::setTopic(std::string newTopic) {
    topic = newTopic;
}

void Channel::setPassword(std::string newPassword) {
    password = newPassword;
}

void Channel::setCapacity(int newCapacity) {
    capacity =newCapacity;
}

//Client Management

bool Channel::isChannelClient(std::string &clientName) {
    if (users.size() == 0)
        return false;
    std::map<std::string, Client>::iterator it = users.find(clientName);
    if (it == users.end())
        return false;
    return true;
}

void Channel::addClient(Client &client) {
    users.insert(std::pair<std::string, Client>(client.getNickName(), client));
}

void Channel::removeClient(std::string &clientName) {
    std::map<std::string, Client>::iterator it = users.find(clientName);
    if (it != users.end())
        users.erase(it);
    removeOperator(clientName);
}

void Channel::addToKicked(std::string &clientName) {
    std::vector<std::string>::iterator it;
    for (it = kickedUsers.begin(); it != kickedUsers.end(); it++) {
        if (*it == clientName) {
            std::cout << clientName << " has already been kicked from channel " << getName() << std::endl;
            return;
        }
    }
    kickedUsers.push_back(clientName);
    std::cout << clientName << "is now kicked from channel " << getName() << std::endl;
    return;
}

//Operators Management

bool Channel::isOperator(std::string operatorName) {
    if(operators.empty())
        return false;
    std::vector<std::string>::iterator it;
    for(it = operators.begin(); it != operators.end(); it++) {
        if (*it == operatorName)
            return true;
    }
    return false;
}

void Channel::addFirstOperator(std::string operatorName) {
    operators.clear();
    operators.push_back(operatorName);
}

void Channel::addOperator(std::string operatorName) {
    std::vector<std::string>::iterator it;
    for(it = operators.begin(); it != operators.end(); it++) {
        if(*it == operatorName) {
            std::cout << operatorName << " is already an operator in channel " << getName() << std::endl;
            return;
        }
    }
    operators.push_back(operatorName);
}

void Channel::removeOperator(std::string operatorName) {
    std::vector<std::string>::iterator it;
    std::cout << "OPER:" << operatorName<<std::endl;
    for(it = operators.begin(); it < operators.end(); it++) {
        if(*it == operatorName)
            operators.erase(it);
    }
}

//Modes Management

void Channel::addMode(std::string const mode) {
    if (mod.empty())
        mod = mode;
    else if(mod.find(mode) == mode.npos)
        mod += mode;
}

void Channel::removeMode(std::string const mode) {
    size_t pos = mod.find(mode);
    std::cout << "MODEIS :" << mod <<std::endl;
    if(pos != mode.npos)
        mod = mod.erase(pos, 1);
}

void Channel::removeChannelPassword() {
    password.clear();
}

void Channel::printClient()
{


    for(std::map<std::string, Client>::iterator it = users.begin(); it !=users.end(); ++it)
    {
        std::cout << "nickname is :" << it->first << " fd is : " << it->second.getFD() << std::endl;
    }
}