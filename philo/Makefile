# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpenas-z <mpenas-z@student.42madrid.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 14:16:05 by mpenas-z          #+#    #+#              #
#    Updated: 2025/05/03 14:29:18 by mpenas-z         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread
DFLAGS = -g -O1
LDFLAGS = -lpthread

SRC_PATH = src/
OBJ_PATH = obj/

SRC =	main.c \
		simulation1.c \
		simulation2.c \
		simulation3.c \
		auxiliary.c \
		utilities.c \
		initializer.c \
		forks.c
SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJS = $(patsubst $(SRC_PATH)%.c,$(OBJ_PATH)%.o,$(SRCS))
INC = -I./inc/

GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

.PHONY: all clean fclean re call

all: $(NAME)

debug: CFLAGS += $(DFLAGS)
debug: $(NAME)

$(NAME): $(OBJS)
	@printf "$(YELLOW)Building $(NAME)...$(RESET) \n"
	@$(CC) $(CFLAGS) $(OBJS) $(INC) $(LDFLAGS) $(CLIBS) -o $(NAME) && \
		printf "$(GREEN)✔ Build succesful!$(RESET) \n" || \
		printf "$(RED)✘ Build failed!$(RESET) \n"

bonus: $(OBJS)
	@printf "$(YELLOW)Building BONUS...$(RESET) \n"
	@$(CC) $(CFLAGS) $(OBJS) $(INC) $(LDFLAGS) $(CLIBS) -o $(NAME) && \
		printf "$(GREEN)✔ Build succesful!$(RESET) \n" || \
		printf "$(RED)✘ Build failed!$(RESET) \n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	@printf "$(BLUE)Compiling $<...$(RESET) \n"
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_PATH):
	@printf "$(BLUE)Creating object directory...$(RESET) \n"
	@mkdir -p $(OBJ_PATH)

clean:
	@printf "$(BLUE)Cleaning object files...$(RESET) \n"
	@rm -f $(OBJS)
	@printf "$(GREEN)✔ Objects cleaned succesfully!$(RESET) \n"

fclean: clean
	@printf "$(BLUE)Removing binaries, dependencies and object files...$(RESET) \n"
	@rm -f $(NAME)
	@printf "$(GREEN)✔ Directory cleaned succesfully!$(RESET) \n"

re: fclean all

call: all clean
	@printf "$(YELLOW)Cleaning dependency builds...$(RESET) \n"
	@printf "$(GREEN)✔ Dependency builds cleaned succesfully!$(RESET) \n"
