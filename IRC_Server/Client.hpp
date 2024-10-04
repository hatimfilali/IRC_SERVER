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
#include <csignal>


class Client
{
    private:
        std::string _NickName;
        std::string _UserName;
        std::string sednBuffer;
        std::string readBuffer;
        int fd;
        bool sendReady;
        bool readReady;
    public:
        char buffer[1024];
        Client();
        Client(const std::string nickname, const std::string username, int fd);
        Client(const Client &other);
        ~Client();
        Client& operator=(const Client &other);

        std::string getNickName();
        std::string getUserName();
        std::string getsednBuffer();
        std::string getReadBuffer();
        bool getsendReady();
        bool getReadReady();
        int getFD();

        void setNickName(const std::string nickname);
        void setUserName(const std::string username);
        void setFD(int fdclient);
        void setsendReady(bool _sendReady);
        void setReadReady(bool ready);
        void setSendBuffer(std::string const &buff);
        void setReadBuffer(const char buffer[1024]);
        std::string SearchNext(std::string searched);
        bool isReady(const char buffer[1024]);
};





#endif