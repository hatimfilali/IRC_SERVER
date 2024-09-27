#include "Client.hpp"


Client::Client(){}
Client::Client(const std::string nickname, const std::string username, int fdclient):_NickName(nickname) , _UserName(username), fd(fdclient), sendReady(false){}
Client::Client(const Client &other)
{
        *this = other;
}
Client::~Client(){}
Client& Client::operator=(const Client &other)
{
    if(this != &other)
    {
        _NickName = other._NickName;
        _UserName = other._UserName;
    }
    return *this;
}

std::string Client::getNickName()
{
    return _NickName;
}

std::string Client::getUserName()
{
    return _UserName;
}
int Client::getFD()
{
    return fd;
}

void Client::setNickName(const std::string nickname)
{
    _NickName = nickname;
}

void Client::setUserName(const std::string username)
{
    _UserName = username;
}
void Client::setFD(int fdcleint)
{
    fd = fdcleint;
}

std::string Client::SearchNext(std::string searched)
{
    char substring[1024] = {0};
    std::string buff(buffer);
    std::size_t found = buff.find(searched);
    int i = 0;
    found += searched.size();
    while(buffer[found] == ' ')
        found++;
    while ((buffer[found] >= 'a' && buffer[found] <= 'z') || (buffer[found] >= 'A' && buffer[found] <= 'Z') || (buffer[found] >= '0' && buffer[found] <= '9'))
    {
        substring[i] = buffer[found];
        i++;
        found++;
    }
    std::string str(substring);
    // std::cout<<"size : +"<<substring[4]<<"+"<<std::endl;
    return(str);
    
}

void Client::setSendBuffer(std::string const &buff) {
    sednBuffer += buff;
}


