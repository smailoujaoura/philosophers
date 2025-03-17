# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/15 11:20:11 by soujaour          #+#    #+#              #
#    Updated: 2025/03/17 12:34:19 by soujaour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

# -O3
# -g -fsanitize=thread
# -g -fsanitize=address
# -O3 -march=native -mtune=native -flto -ffast-math -fstrict-aliasing -funroll-loops -fprefetch-loop-arrays -fomit-frame-pointer -fno-stack-protector

SRCS =	main.c philo.c init.c time.c atoi.c

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

re: fclean all

.PHONY: clean run