# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/15 11:20:11 by soujaour          #+#    #+#              #
#    Updated: 2025/02/19 11:59:54 by soujaour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
NAME_B = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
# pthread as well

SRCS =	main.c utils.c more.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@ -MMD

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS:.o=.d)

fclean: clean
	rm -rf $(NAME)

run:
	./philo

re: fclean all

.PHONY: clean run