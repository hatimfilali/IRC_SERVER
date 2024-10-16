#ifndef MACROS_HPP
#define MACROS_HPP

#define SUCSSES 0
#define FAILURE -1
#define BREAK 2
#define CONTINUE 3
#define VALID_LENGTH 7

#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

//PARSING
#define ERR_UNKNONKCOMMAND(client, command) (":localhost 421 " + client + " #" + command + " :Unknown command\r\n")

//PASS
# define ERR_INVALIDPASS(client) (":localhost 464 " + client + " :Password incorrect.\r\n")

//INVITE
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough patameters.\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " #" + channel + " :No such channel.\r\n")
#define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " #" + channel + " :The user is not on this channel.\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) ( ":localhost 443 " + client + " " + nick + " #" + channel + " :The user already on channel.\r\n")
#define RPL_INVITING(user_id, client, nick, channel) (user_id + " 341 " + client + " " + nick + " #" + channel + "\r\n")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\\r\n")

//JOIN
#define RPL_JOIN(nickname, channel) (":" + nickname + " JOIN :#" +channel + "\r\n")
#define ERR_BANNEDFROMCHANNEL(client, channel) ("474 " + client + " #" + channel + " :Cannot join chaannel (+b)\r\n")
#define ERR_BADCHAANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + " #" + channel + " :Cannot joinn channel (+l)\r\n")
#define RPL_NAMEREPLY(client, channel, list_of_members) (":localhost 353 " + client + " = #" + channel + " :" + list_of_members + "\r\n")
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")

//TOPIC
#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
#define ERR_NOCHANNELGIVEN(client, command) (client + " " + command + " :No channel given\r\n")
#define RPL_CHANNELHASNOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set.\r\n")


//PRIVMSG
#define ERR_NORECIPIENT(client) ("411 " + client + " :No recipieent given PRIVMSG\r\n")
#define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
#define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
#define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + " PRIVMSG " + target + " :" + message + "\r\n")


//KICK
#define ERR_USERNOTINCHANNEL(client, nickname, channel) ("441 " + client + " " + nickname + " #" + channel + " :They arn't on that channel\r\n")
#define ERR_NOTCHANNELOP(client, channel) (":localhost 482 " + client + " #" + channel + " :You are not the channel operator\r\n")
#define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK #" + channel + " " + kicked + " " + reason + "\r\n")

//BAN
#define RPL_BANNED(user_id, channel, banned, reson) (user_id + "BANNED #" + channel + " " + banned + " " + reason + "\r\n")

//NICK
#define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
#define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE( nickname) (":localhost 433 " + nickname + " :Nickname is already in use.\r\n")


#define CHECK_STRING_SIZE(str) ((strlen(str) > 1024) ? 1024 : strlen(str))

#define ERR_BADMODEPARAM(client, command) (client + " " + command + " :Bad parameters \r\n")
#define ERR_CLIENTALREDYOP(client, channel, nickname) (client + " #" + channel + " " + nickname + " :Already a channel operator\r\n")
#define RPL_ADDEDOPERATOR(client, channel, nickname) ( client + " #" + channel + " " + nickname + " :Operator added\r\n")
#define RPL_REMOVEDOPERATOR(client, channel, nickname) ( client + " #" + channel + " " + nickname + " :Operator removed\r\n")

#define RPL_CHANNELMODES(nickname, channel, modes) (":localhost 324 " + nickname + " #" + channel + " " + modes + "\r\n")



#endif



