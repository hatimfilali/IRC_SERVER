#include <iostream>
#include "Server.hpp"

int main(int ac, char** av)
{
    if(ac != 3)
    {
        std::cout << "You need to run the executable as follows : \n./ircserv <port> <password>" <<std::endl;
        std::cout << av[0] << std::endl;
        return 1;
    }

    Server server(av[2], std::atoi(av[1]));
    try
    {
        server.BindingAdress();
        server.Listening();
        while(true)
        {
            server.AddTo_FD_Set();
            server.CheckForIncomingConnection();
            server.GetMsgFromClients();
            
        }
        server.CloseSocket();
    }catch( const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        server.CloseSocket();
    }
























    // char buffer[1024] = {0};
    // int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_socket == -1) {
    //     std::cerr << "Failed to create socket" << std::endl;
    //     return 1;
    // }
    // // Define server address
    // struct sockaddr_in server_address;
    // std::memset(&server_address, 0, sizeof(server_address)); // Zero out the structure
    // server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    // server_address.sin_port = htons(6666); // Bind to port 6667

    // // Bind socket to the specified IP and port
    // if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    //     std::cerr << "Failed to bind to port 6667" << std::endl;
    //     close(server_socket);
    //     return 1;
    // }

    // // Listen for incoming connections
    // if (listen(server_socket, 10) == -1) {
    //     std::cerr << "Failed to listen on socket" << std::endl;
    //     close(server_socket);
    //     return 1;
    // }
    // fd_set read_fds;         // Set of file descriptors to monitor for reading
    // int max_fd;              // Maximum file descriptor number
    // struct timeval timeout;  
    // int arr[5] = {0};
    // // while (true)
    // // {
    // //     FD_ZERO(&read_fds);                // Clear the set
    // //     FD_SET(server_socket, &read_fds);  // Add server socket to the set
    // //     //ajouter le socket de client au read fd set
    // //     //for client in clients :
    // //     //fd_set(client.socket, &read_fds)
    // //     max_fd = server_socket;

    // //     timeout.tv_sec = 5;
    // //     timeout.tv_usec = 0;

    // //     // Use select to monitor sockets
    // //     int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

    // //     if (activity < 0) {
    // //         std::cerr << "Select error" << std::endl;
    // //         continue;
    // //     }

    // //     // Check if the server socket is ready (new incoming connection)
    // //     if (FD_ISSET(server_socket, &read_fds)) {
    // //         int new_socket = accept(server_socket, NULL, NULL);
    // //         arr[0]= new_socket;
    // //         if (arr[0] >= 0) {
    // //             // clients[new_socket] = new_socket;  // Store the new client socket
    // //             ssize_t bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);
    // //             if (bytes_received < 0) {
    // //                 // perror("recv failed");
    // //             } else if (bytes_received == 0) {
    // //                 std::cout << "Client disconnected" << std::endl;
    // //             } else {
    // //                 // Print the received message
    // //                 // buffer[bytes_received] = '\0';  // Null-terminate the buffer to print as a string
    // //                 std::cout << "Received from client: " << buffer << std::endl;
    // //             }

    // //             std::cout << "New connection on socket " << new_socket << std::endl;
    // //         }
    // //     }
        

    // // }


    // while (true)
    // {
    //     FD_ZERO(&read_fds);                // Clear the set
    //     FD_SET(server_socket, &read_fds);  // Add server socket to the set
    //     //ajouter le socket de client au read fd set
    //     //for client in clients :
    //     //fd_set(client.socket, &read_fds)
    //     max_fd = server_socket;
    //     if(arr[0])
    //     {
    //         max_fd = arr[0];
    //         FD_SET(arr[0], &read_fds);
    //         // std::cout << "zabi rani dkhalt hak :" << arr[0]<< std::endl;
    //     }
    //     timeout.tv_sec = 5;
    //     timeout.tv_usec = 0;

    //     // Use select to monitor sockets
    //     int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
    //     // std::cout << "zabi rani tal9a"<< std::endl;
    //     if (activity < 0) {
    //         std::cerr << "Select error" << std::endl;
    //         continue;
    //     }

    //     // Check if the server socket is ready (new incoming connection)
    //     if (FD_ISSET(server_socket, &read_fds)) {
    //         int new_socket = accept(server_socket, NULL, NULL);
    //         arr[0]= new_socket;
    //         std::cout << "New connection on socket " << new_socket << std::endl;
    //         }
    //     if (FD_ISSET(arr[0], &read_fds)) {
    //         // int new_socket = accept(server_socket, NULL, NULL);
    //         // arr[0]= new_socket;

            
    //         if (arr[0] >= 0) {
    //             // clients[new_socket] = new_socket;  // Store the new client socket
    //             std::cout << "zabi rani dkhalt "<< std::endl;
    //             ssize_t bytes_received = recv(arr[0], buffer, sizeof(buffer), 0);
    //             std::cout << "zabi rani dkhalt2 "<< std::endl;
    //             if (bytes_received < 0) {
    //             } else if (bytes_received == 0) {
    //                 std::cout << "Client disconnected" << std::endl;
    //             } else {
    //                 // Print the received message
    //                 // buffer[bytes_received] = '\0';  // Null-terminate the buffer to print as a string
    //                 std::cout << "Received from client: " << buffer << std::endl;
    //             }
    //         }
    //     }
    //     }

    // close(server_socket);

}