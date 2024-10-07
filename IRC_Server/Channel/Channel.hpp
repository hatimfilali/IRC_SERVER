#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Client.hpp"
#include <vector>

class Client;
class Channel {
    private:   
        std::string ChannelName;
        std::string topic;
        std::string mod;
        std::string password;
        std::string operatorPassword;
        int capacity;
        std::map<std::string, Client> users;
        std::vector<std::string> bannedUsers;
        std::vector<std::string> kickedUsers;
        std::vector<std::string> operators;

    public:
        Channel(std::string const &name);
        ~Channel();

        void printClient();

        //Getters and Setters
        std::string                        &getName();
        std::string                     &getTopic();
        std::string                     &getMod();
        std::string                     &getPassword();
        std::string                     &getOperatorPassword();
        int                             &getCapacity();
        std::map<std::string, Client>   &getUsers();
        std::vector<std::string>    &getBannedUsers();
        std::vector<std::string>    &getKickedUsers();
        std::vector<std::string>    &getOperators();
        void                        setTopic(std::string &newTopic);
        void                        setPassword(std::string &newTassword);
        void                        setCapacity(int &newTapacity);

        //Clients Management
        bool                        isChannelClient(std::string &clientName);
        void                        addClient(Client &client);
        void                        removeClient(std::string &clientName);

        void                        addToKicked(std::string &clientName);
        void                        addToBanned(std::string &clientName);
        void                        removeFromBanned(std::string &clientName);
        bool                        isBanned(std::string &clientName);

        //Operators Management
        void                        addFirstOperator(std::string operatorName);
        bool                        isOperator(std::string operatorName);
        void                        addOperator(std::string operatorName);
        void                        removeOperator(std::string operatorName);

        //Modes Management
        void                        addMode(std::string const mode);
        void                        removeMode(std::string const mode);
        void                        removeChannelPassword();




        

};


#endif