NAME = ircserv
SRCS = main.cpp Server.cpp Client.cpp Channel/Channel.cpp Commands/invite.cpp Commands/join.cpp Commands/kick.cpp Commands/privmsg.cpp Commands/tools.cpp Commands/topic.cpp Commands/mode.cpp
OBJS = $(SRCS:.cpp=.o)
CPP = c++
CPPFLAGS =-std=c++98 -fsanitize=address -g3

all: $(NAME)

%.o:%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all