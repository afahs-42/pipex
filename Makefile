# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afahs <afahs@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/22 12:53:50 by afahs             #+#    #+#              #
#    Updated: 2025/07/30 19:44:35 by afahs            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += -s

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRCS = src/pipex.c \
	   src/ft_split.c \
	   src/utils.c \
	   src/ft_string.c \
	   src/processes.c \
	   src/here_doc.c \

Inculde = include

OBJS = $(SRCS:.c=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re