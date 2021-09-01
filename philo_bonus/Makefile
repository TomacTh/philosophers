CC		=	gcc
NAME	=	philo
NAME_BONUS =	philo_bonus
CFLAGS	= 	-Wall -Wextra -Werror
INCLUDES		=	./inc
SRCS = $(addprefix ./srcs/, ft_create_philos.c ft_utils.c ft_philosophers.c ft_parse_and_init.c ft_monitoring.c )


OBJS	=	$(SRCS:.c=.o)
OBJSBONUS =	$(SRCSBONUS:.c=.o)

all:	$(NAME)

bonus: $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(NAME_BONUS):	$(OBJSBONUS)
	$(CC) $(CFLAGS) $(OBJSBONUS) -o $(NAME_BONUS)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJSBONUS)

fclean:	clean
	rm -f $(NAME) $(NAME_BONUS)


re:	fclean all

.PHONY:	all bonus clean re install
