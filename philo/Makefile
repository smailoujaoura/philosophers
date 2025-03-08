# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/15 11:20:11 by soujaour          #+#    #+#              #
#    Updated: 2025/03/08 22:18:30 by soujaour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
NAME_B = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -o3 #-g -fsanitize=thread


SRCS =	main.c utils.c more.c writing.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

run:
	./philo

re: fclean all

.PHONY: clean run