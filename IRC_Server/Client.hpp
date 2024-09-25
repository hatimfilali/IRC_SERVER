#ifndef Client_HPP
#define Client_HPP

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <map>
#include <cstdlib>


class Client
{
    private:
        std::string _NickName;
        std::string _UserName;
        std::string sednBuffer;
        int fd;
    public:
        char buffer[1024];
        Client();
        Client(const std::string nickname, const std::string username, int fd);
        Client(const Client &other);
        ~Client();
        Client& operator=(const Client &other);

        std::string getNickName();
        std::string getUserName();
        int getFD();
        void setNickName(const std::string nickname);
        void setUserName(const std::string username);
        void setFD(int fdclient);
        void setSendBuffer(std::string const &buff);
        std::string SearchNext(std::string searched);
};





#endif