#ifndef MACROS_HPP
#define MACROS_HPP

#define SUCSSES 0
#define FAILURE -1
#define BREAK 2
#define CONTINUE 3

#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")


//INVITE
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough patameters.\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " #" + channel + " :No such channel.\r\n")
#define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " #" + channel + " :The user is not on this channel.\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) ( ":localhost 443 " + client + " " + nick + " #" + channel + " :The user already on channel.\r\n")
#define RPL_INVITING(user_id, client, nick, channel) (user_id + " 341 " + client + " " + nick + " #" + channel + "\r\n")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\\r\n")

//JOIN
#define RPL_JOIN(user_id, channel) (user_id + " JOOIN :#" +channel + "\r\n")
#define ERR_BANNEDFROMCHANNEL(client, channel) ("474 " + client + " #" + channel + " :Cannot join chaannel (+b)\r\n")
#define ERR_BADCHAANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + " #" + channel + " :Cannot joinn channel (+l)\r\n")
#define RPL_NAMEREPLY(client, channel, list_of_members) (":localhost 353 " + client + " #" + channel + " :" + list_of_members + "\r\n")
#define RPL_ENDOFNAMES(client, channel) (":localhost " + client + " #" + channel + " :End of names list\r\n")
//TOPIC
#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")



#endif

