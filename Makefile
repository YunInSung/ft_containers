SRCS			= test.cpp
OBJS			= $(SRCS:.cpp=.o)

CXX				= c++
RM				= rm -rf
CXXFLAGS		= -std=c++98 -Wall -Wextra -Werror -g

NAME			= exc

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re