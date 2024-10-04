#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel/Channel.hpp"
#include "Commands/Commands.hpp"


class Server
{
    private:
        int _Port;
        int _ServerSocket;
        int max_fd;
        std::string _Password;
        fd_set read_fds;
        struct sockaddr_in server_address;
        struct timeval timeout;
        std::map<int, Client> _Clients;
        std::map<std::string, Channel> _Channels;



    public:
        Server();
        Server(std::string Password, int Port);
        ~Server();
        void setPassword(std::string Password);
        void setPort(int Port);
        std::map<std::string, Channel> &getChannels();
        std::map<int, Client> &getClients();
        void BindingAdress();
        void Listening();
        void AddTo_FD_Set();
        void CheckForIncomingConnection();
        bool CheckUserExist(std::string Nickname);
        void GetMsgFromClients();
        void SendResponse();
        void CloseSocket();
};

#endif