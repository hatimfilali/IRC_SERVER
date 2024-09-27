#include "Server.hpp"

Server::Server(std::string Password, int Port):_Password(Password)
{
    if(Port < 1024 || Port > 49152)
        throw std::runtime_error("Invalid Port \nChose Port Between 1024 And 49152");
    _Port = Port;
    _ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_ServerSocket == -1)
        throw std::runtime_error("Failed to create socket");
}

std::map<std::string, Channel> &Server::getChannels() {
    return _Channels;
}

std::map<int, Client> &Server::getClients() {
    return _Clients;
}

void Server::BindingAdress()
{
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(_Port);
    if (bind(_ServerSocket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) 
    {
        std::ostringstream oss;
        oss << "Failed to bind to port: " << _Port;
        throw std::runtime_error(oss.str());
    }
}

void Server::Listening()
{
    if (listen(_ServerSocket, SOMAXCONN) == -1)
        throw std::runtime_error("Failed to listen on socket");
}

void Server::AddTo_FD_Set()
{
    FD_ZERO(&read_fds);                
    FD_SET(_ServerSocket, &read_fds);

    max_fd = _ServerSocket;
    for(std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        FD_SET(it->first, &read_fds);
        if(it->first > max_fd)
            max_fd = it->first;
    }
}

void Server::CheckForIncomingConnection()
{
    Client client;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
    if(activity < 0)
        std::runtime_error("An error occurred in select()");
    if (FD_ISSET(_ServerSocket, &read_fds)) {
        int new_socket = accept(_ServerSocket, NULL, NULL);
        
        if(new_socket >= 0)
        {
            ssize_t bytes_received = recv(new_socket, client.buffer, sizeof(client.buffer), 0);
            if (bytes_received < 0)
                std::runtime_error("recv Failed");
            else if (bytes_received == 0)
            {
                close(new_socket);
                _Clients.erase(new_socket);
                std::cout << "Client disconnected" << std::endl;
            }else
            {
                if(_Password != client.SearchNext("PASS"))
                {
                    close(new_socket);
                    std::cout << "Invalid Password : {" <<client.SearchNext("PASS")<< "}" <<std::endl;
                    // std::cout << "valid Password : {" <<_Password<<"}" <<std::endl;
                }else
                {
                    client.setNickName(client.SearchNext("NICK"));
                    client.setUserName(client.SearchNext("USER"));
                    client.setFD(new_socket);
                    _Clients[new_socket] = client;
                    std::cout << "New connection on socket " << new_socket << std::endl;    
                }
            }
        }



    }
}

void Server::GetMsgFromClients()
{
    for(std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        if (FD_ISSET(it->first, &read_fds))
        {
            ssize_t bytes_received = recv(it->first, it->second.buffer, sizeof(it->first, it->second.buffer), 0);
            if(bytes_received < 0)
                std::cout << "Got error while receiving data from client"<<std::endl;
            else if(bytes_received == 0)
                std::cout << "Client disconnected" << std::endl;
            else
            {
                //hna radi dir l code nta3ek
                getCommandLine(this, it->first, it->second.buffer);
                
                std::cout << "Received from client: " << it->second.buffer << std::endl;
            }
        }
    }
}

void Server::SendResponse()
{
    for(std::map<int,Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        if(it->second.getsendReady() == true)
        {
            ssize_t bytes_sent = send(it->first, it->second.getsednBuffer().c_str(), strlen(it->second.getsednBuffer().c_str()), 0);
            if (bytes_sent < 0) {
                std::cout << "Got error while sending data to client"<<std::endl;
            }
            it->second.setsendReady(false);
        }
    }
}

void Server::CloseSocket()
{
    for(std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
        close(it->first);
    close(_ServerSocket);
}
Server::~Server()
{
    CloseSocket();
}