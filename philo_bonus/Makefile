NAME = philo_bonus
CC = cc
CFLAGS = -pthread -g -fsanitize=thread
# -Wall -Wextra -Werror
SRCS =	main_bonus.c init_bonus.c philo_bonus.c rand_bonus.c time_bonus.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean