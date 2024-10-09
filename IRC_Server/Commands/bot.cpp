#include "Commands.hpp"

void bot(Server &server, int client_fd, cmd_struct cmd_info)
{
    Client &client = server.getClients()[client_fd];  // Retrieve the client using client_fd
    std::string arg = cmd_info.msg;  // Assuming cmd_info.msg contains the argument (question number)

    if (arg.empty())
    {
        std::string res = "Welcome " + client.getNickName() + " to our IRC Server \r\n";
        addToClientBuffer(&server, client_fd, res);

        std::string res1 = "These are the available questions you can ask me. Please select the number of the question:\n";
        std::string res2 = "1- Who are the creators of this server?\n";
        std::string res3 = "2- What are the commands that you provide?\n";
        std::string res4 = "3- What are the available channels?\r\n";
        std::string full_res = res1 + res2 + res3 + res4;
        addToClientBuffer(&server, client_fd, full_res);
    }
    else
    {
        if (arg == "1")
        {
            std::string res = "The creators of this server are: HATIM FILALI, SAAD MBARKI and OMAR FARHAT\r\n";
            addToClientBuffer(&server, client_fd, res);
        }
        else if (arg == "2")
        {
            std::string res = "The commands that you can use are as follow:\r\n";
            std::string res1 = "1- /join <channel> : to join a channel\n";
            std::string res2 = "2- /kick <channel> <nickname> : to kick a member from a channel\n";
            std::string res3 = "3- /invite <nickname> <channel> : to invite a member to a channel\n";
            std::string res4 = "4- /topic <channel> <topic> : to set a topic to a channel\n";
            std::string res5 = "5- /mode <channel> <mode> : to set a mode to a channel\r\n";
            std::string full_res = res + res1 + res2 + res3 + res4 + res5;
            addToClientBuffer(&server, client_fd, full_res);
        }
        else if (arg == "3")
        {
            std::string res = "The available channels are:\r\n";
            for (std::map<std::string, Channel>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
            {
                res += it->first + "\n";  // Channel name is the key of the map
            }
            res += "\r\n";
            addToClientBuffer(&server, client_fd, res);
        }
        else
        {
            std::string res = "Invalid question. Please select an appropriate number for your question.\r\n";
            addToClientBuffer(&server, client_fd, res);
        }
    }
}

