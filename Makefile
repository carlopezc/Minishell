# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/27 15:14:10 by carlopez          #+#    #+#              #
#    Updated: 2025/10/20 15:17:02 by carlopez         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

SHELL	:= /bin/bash
BONUS	= minishell_bonus
AR	= ar
ARFLAGS	= -rcs
CC	= cc

INCLUDES = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib
LDLIBS = -lreadline -lhistory -lcurses
CFLAGS	= -g -Wall -Wextra -Werror -fsanitize=address 
OFLAGS	= -MMD -MF $(@:.o=.d)

# Directorios
SRCDIR   = src
BSRCDIR	 = bonus
UTILSDIR = utils
DEPDIR	 = deps
OBJDIR	 = objs
BOBJDIR	 = bonus_obj
PRINTFDIR	= printf
OUTILSDIR	= utils_objects
OBJ_GETDIR	= objs/get_next_line
GETDIR	        = get_next_line

# Archivos de cabecera
LIB	= header/ft_minishell.h
LIBGET	= $(GETDIR)/get_next_line_bonus.h

# Archivos fuente
SRC	= ft_get_elements2.c ft_token_utils2.c ft_spaces.c ft_if.c ft_parsing2.c ft_find.c ft_operators.c ft_is_operator2.c ft_check_brackets.c ft_wildcard_helpers.c ft_wildcard_helpers2.c ft_aux_export.c ft_is_operator.c ft_unquote_if.c ft_unquote_inside.c ft_unquote_inside2.c ft_asterisk_mini.c ft_aux_aux_check.c ft_aux_brackets.c ft_variable.c ft_free_aux.c ft_asterisk.c ft_file_management.c ft_manage_nodes.c ft_quotes.c ft_str_utils.c ft_tokenization.c ft_parsing.c ft_executor.c ft_split_cmd.c ft_wildcard.c ft_aux_check.c ft_check.c ft_count.c ft_env.c ft_expand.c ft_export.c ft_free.c ft_get_elements.c ft_init.c ft_measurements.c ft_print.c ft_signals.c ft_strdup_variants.c ft_testing_tools.c ft_token_utils.c ft_brackets.c ft_executor2.c ft_executor3.c ft_executor4.c ft_executor5.c ft_executor6.c ft_executor7.c ft_executor8.c ft_executor9.c ft_path.c ft_redir.c ft_analisis.c ft_executor13.c ft_executor14.c ft_echo.c ft_heredoc.c ft_executor16.c ft_extras.c
BSRC	= ft_get_elements2.c ft_token_utils2.c ft_spaces.c ft_if.c ft_parsing2.c ft_find.c ft_operators.c ft_is_operator2.c ft_check_brackets.c ft_wildcard_helpers.c ft_wildcard_helpers2.c ft_aux_export.c ft_is_operator.c ft_unquote_if.c ft_unquote_inside.c ft_unquote_inside2.c ft_asterisk_mini.c ft_aux_aux_check.c ft_aux_brackets.c ft_variable.c ft_free_aux.c ft_asterisk.c ft_file_management.c ft_manage_nodes.c ft_quotes.c ft_str_utils.c ft_tokenization.c ft_parsing.c ft_executor.c ft_split_cmd.c ft_wildcard.c ft_aux_check.c ft_check.c ft_count.c ft_env.c ft_expand.c ft_export.c ft_free.c ft_get_elements.c ft_init.c ft_measurements.c ft_print.c ft_signals.c ft_strdup_variants.c ft_testing_tools.c ft_token_utils.c ft_brackets.c ft_executor2.c ft_executor3.c ft_executor4.c ft_executor5.c ft_executor6.c ft_executor7.c ft_executor8.c ft_executor9.c ft_path.c ft_redir.c ft_analisis.c ft_executor13.c ft_executor14.c ft_echo.c ft_heredoc.c ft_executor16.c ft_extras.c
UTILS	= ft_utils.c
GET	= get_next_line_bonus.c get_next_line_utils_bonus.c

# Archivos de objetos
OBJS       = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
BOBOJ	   = $(addprefix $(BOBJDIR)/, $(BSRC:.c=.o))
DEPS       = $(addprefix $(DEPDIR)/, $(SRC:.c=.d) $(UTILS:.c=.d) $(GET:.c=.d))
OUTILS	   = $(addprefix $(OUTILSDIR)/, $(UTILS:.c=.o))
OBJS_GET   = $(addprefix $(OBJ_GETDIR)/, $(GET:.c=.o))

# Biblioteca
LIBPRINTF  = $(PRINTFDIR)/libftprintf.a

# Archivo principal
MAIN       = src/main.c
BMAIN	   =
# Colores
RED        = \033[0;31m
ORANGE	   = \033[38;5;208m
GREEN      = \033[0;32m
YELLOW     = \033[0;33m
BLUE       = \033[0;34m
PURPLE     = \033[0;35m
CYAN       = \033[0;36m
RESET      = \033[m
DARKGREEN = \033[38;5;22m

# Objetivo principal
all: $(NAME) $(LIB) $(LIBGET) Makefile

-include $(DEPS)

# Compilación de archivos fuente generales
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c Makefile | $(OBJDIR) $(DEPDIR)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	$(CC) $(CFLAGS) $(INCLUDES) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

$(BOBJ): $(BOBJDIR)/%.o : $(BSRCDIR)/%.c Makefile | $(BOBJDIR) $(DEPDIR)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

$(OBJS_GET): $(OBJ_GETDIR)/%.o : $(GETDIR)/%.c Makefile | $(OBJ_GETDIR) $(DEPDIR)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJ_GETDIR)/*.d $(DEPDIR)

$(OUTILS): $(OUTILSDIR)/%.o : $(UTILSDIR)/%.c Makefile | $(OUTILSDIR) $(DEPSDIR)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OUTILSDIR)/*.d deps/

# Creación de directorios
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJ_GETDIR):
	@mkdir -p $(OBJ_GETDIR)

$(OUTILSDIR):
	@mkdir -p $(OUTILSDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Compilación de la biblioteca printf
$(LIBPRINTF):
	@: printf "%b" "$(BLUE)$(@F)$(RESET)\n"
	@$(MAKE) --silent -C $(PRINTFDIR)

$(NAME): $(MAIN) $(OBJS) $(OBJS_GET) $(OUTILS) $(LIBPRINTF)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(MAIN) $(OBJS) $(OBJS_GET) $(OUTILS) $(LIBPRINTF) -o $(NAME) $(LDFLAGS) $(LDLIBS)

bonus: $(BONUS) $(LIBPRINTF) $(LIB) Makefile

$(BONUS): $(MAIN) $(OBJS) $(OBJS_GET) $(OUTILS) #$(LIBPRINTF)
	@: printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(MAIN) $(OBJS) $(OBJS_GET) $(OUTILS) $(LIBPRINTF) -o $(NAME) $(LDFLAGS) $(LDLIBS)

# Limpiar objetos y dependencias
clean:
	@: printf "%b" "$(BLUE)Cleaning objects...$(RESET)\n"
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@rm -rf $(OBJ_GETDIR)
	@rm -rf $(OUTILSDIR)
	@$(MAKE) -C $(PRINTFDIR) clean --silent

# Limpiar todo
fclean: clean
	@: printf "%b" "$(BLUE)Cleaning all files...$(RESET)\n"
	@rm -f $(NAME)
	@rm -rf $(BONUS)
	@$(MAKE) -C $(PRINTFDIR) fclean --silent

# Reconstrucción total
re: fclean all

.PHONY: all clean fclean re $(LIBPRINTF) bonus
