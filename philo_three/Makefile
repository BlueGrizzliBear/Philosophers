# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/30 10:08:08 by cbussier          #+#    #+#              #
#    Updated: 2020/11/26 15:01:25 by cbussier         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

HEADER			=		./includes/philo_three.h

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

NAME			=		philo_three

FLAGS			=		-Wall -Wextra -Werror -I.#-fsanitize=address

LIB_LINK		=		-lpthread

SRCS			=		$(addprefix $(SRC_PATH)/,$(SRCS_NAME))

all				:		philo_three $(NAME)

%.o				:		%.c $(HEADER)
						gcc $(FLAGS) -c $< -o $(<:.c=.o)

$(NAME)			:		$(OBJS)
						gcc $(FLAGS) $(LIB_LINK) $(OBJS) -o $(NAME)

clean			:
						rm -f $(OBJS)

fclean			:		clean
						rm -f $(NAME)

re				:      	fclean all

.PHONY			:		all clean fclean re
