CC		=	gcc
NAME	=	philo
NAME_BONUS =	philo_bonus
CFLAGS	= 	-Wall -Wextra -Werror
INCLUDES		=	./inc
SRCS = $(addprefix ./srcs/, ft_create_philos.c ft_utils.c ft_philosophers.c ft_parse_and_init.c ft_monitoring.c )
SRCSBONUS = $(addprefix ./srcsbonus/, ft_create_philos_bonus.c ft_utils_bonus.c ft_philosophers_bonus.c ft_parse_and_init_bonus.c ft_monitoring_bonus.c )

OBJS	=	$(SRCS:.c=.o)
OBJSBONUS =	$(SRCSBONUS:.c=.o)

all:	$(NAME)

bonus: $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

$(NAME_BONUS):	$(OBJSBONUS)
	$(CC) $(CFLAGS) $(OBJSBONUS) -lpthread -o $(NAME_BONUS)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJSBONUS)

fclean:	clean
	rm -f $(NAME) $(NAME_BONUS)


re:	fclean all

.PHONY:	all bonus clean re install
