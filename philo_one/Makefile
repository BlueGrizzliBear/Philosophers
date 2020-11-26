# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/30 10:08:08 by cbussier          #+#    #+#              #
#    Updated: 2020/11/26 12:00:46 by cbussier         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

HEADER			=		./includes/philo_one.h

SRC_PATH		=		srcs

SRCS_NAME		=		main.c \
						parse.c \
						utils.c \
						error.c \
						init.c \
						alive.c \
						display.c \
						free.c

OBJS			= 		${SRCS:.c=.o}

NAME			=		philo_one

FLAGS			+=		-Wall -Wextra -Werror -I -lpthread #-fsanitize=address

SRCS			=		$(addprefix $(SRC_PATH)/,$(SRCS_NAME))

all				:		philo_one $(NAME)

%.o				:		%.c $(HEADER)
						gcc $(FLAGS) -c $< -o $(<:.c=.o)

$(NAME)			:		$(OBJS)
						gcc $(FLAGS) $(OBJS) -o $(NAME)

clean			:
						rm -f $(OBJS)

fclean			:		clean
						rm -f $(NAME)

re				:      	fclean all

.PHONY			:		all clean fclean re
