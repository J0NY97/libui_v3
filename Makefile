# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 11:13:50 by nneronin          #+#    #+#              #
#    Updated: 2021/12/18 11:47:53 by jsalmi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL_NAME	:= $(shell uname -s)

all:
ifeq ($(SHELL_NAME), Darwin)
	@sh ui_path_mac.sh
	@make -f Makefile-mac -j6
else ifeq ($(SHELL_NAME), Linux)
	@echo "OS: $(SHELL_NAME)"
	@echo "Currently not supporting Lunix version."
else
	@echo "OS: $(SHELL_NAME)"
	@sh ui_path_win.sh
	@echo "Making windows version."
	@mingw32-make -f Makefile-win -j6
endif

clean:
ifeq ($(SHELL_NAME), Darwin)
	@make clean -f Makefile-mac
else
	@mingw32-make clean -f Makefile-win
endif
	@rm ui_path.h

fclean:
ifeq ($(SHELL_NAME), Darwin)
	@make fclean -f Makefile-mac
else
	@mingw32-make fclean -f Makefile-win
endif

re: fclean all

.PHONY: clean, all, re, fclean, framework, framework_del, frameworks_re
