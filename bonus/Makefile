# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 14:39:07 by soujaour          #+#    #+#              #
#    Updated: 2025/03/18 15:00:38 by soujaour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

# -O3
# -g -fsanitize=thread
# -g -fsanitize=address
# -O3 -march=native -mtune=native -flto -ffast-math -fstrict-aliasing -funroll-loops -fprefetch-loop-arrays -fomit-frame-pointer -fno-stack-protector

SRCS =	main_bonus.c init_bonus.c philo_bonus.c rand_bonus.c time_bonus.c

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