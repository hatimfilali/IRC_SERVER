#include "Client.hpp"


Client::Client()
{
    sednBuffer.clear();
    fd = -1;
}
Client::Client(const std::string nickname, const std::string username, int fdclient):_NickName(nickname) , _UserName(username), fd(fdclient), sendReady(false), readReady(false)
{
    sednBuffer.clear();
    readBuffer.clear();
}
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
        fd = other.fd;
        sendReady = other.sendReady;
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
std::string Client::getsednBuffer()
{
    return sednBuffer;
}
std::string Client::getReadBuffer()
{
    return readBuffer;
}
int Client::getFD()
{
    return fd;
}
bool Client::getsendReady()
{
    return sendReady;
}
bool Client::getReadReady()
{
    return readReady;
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
void Client::setsendReady(bool _sendReady)
{
    this->sendReady = _sendReady;
}
void Client::setReadBuffer(const char buffer[1024])
{
    readBuffer += buffer;
}
void Client::setReadReady(bool ready)
{
    readReady = ready;
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
    return(str);
    
}

void Client::setSendBuffer(std::string const &buff) {
    sednBuffer = buff;
}

bool Client::isReady(const char buffer[1024])
{
    const char* result = strstr(buffer, "\n\r");
    if(result)
        return true;
    return false;
}


