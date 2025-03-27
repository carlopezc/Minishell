# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/27 15:14:10 by carlopez          #+#    #+#              #
#    Updated: 2025/03/27 21:00:10 by lbellmas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME       = minishell
 
BONUS		= minishell_bonus
AR         = ar
ARFLAGS    = -rcs
CC         = cc

RFLAGS	= -L/usr/local/lib -I/usr/local/include -lreadline -lncurses
CFLAGS     = -Wall -Wextra -Werror #-fsanitize=address 
OFLAGS     = -MMD -MF $(@:.o=.d)

# Directorios
SRCDIR     = src
BSRCDIR		= bonus
UTILSDIR   = utils
DEPDIR     = deps
OBJDIR     = objs
BOBJDIR		= bonus_obj
PRINTFDIR  = printf
OUTILSDIR	= utils_objects

# Archivos de cabecera
LIB        = header/ft_minishell.h

# Archivos fuente
SRC        = ft_tokenization.c ft_executor.c
BSRC		= 
UTILS      = ft_utils.c

# Archivos de objetos
OBJS       = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
BOBOJ		= $(addprefix $(BOBJDIR)/, $(BSRC:.c=.o))
DEPS       = $(addprefix $(DEPDIR)/, $(SRC:.c=.d) $(UTILS:.c=.d) $(GET:.c=.d))
OUTILS	= $(addprefix $(OUTILSDIR)/, $(UTILS:.c=.o))

# Biblioteca
LIBPRINTF  = $(PRINTFDIR)/libftprintf.a

# Archivo principal
MAIN       = src/main.c
BMAIN		=
# Colores
RED        = \033[0;31m
GREEN      = \033[0;32m
YELLOW     = \033[0;33m
BLUE       = \033[0;34m
PURPLE     = \033[0;35m
CYAN       = \033[0;36m
RESET      = \033[m

# Objetivo principal
all: $(LIBPRINTF) $(NAME) $(LIB) Makefile

-include $(DEPS)

# Compilación de archivos fuente generales
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c Makefile | $(OBJDIR) $(DEPDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

$(BOBJ): $(BOBJDIR)/%.o : $(BSRCDIR)/%.c Makefile | $(BOBJDIR) $(DEPDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

$(OUTILS): $(OUTILSDIR)/%.o : $(UTILSDIR)/%.c Makefile | $(OUTILSDIR) $(DEPSDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OUTILSDIR)/*.d deps/

# Creación de directorios
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OUTILSDIR):
	@mkdir -p $(OUTILSDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Compilación de la biblioteca printf
$(LIBPRINTF):
	@printf "%b" "$(BLUE)$(@F)$(RESET)\n"
	@$(MAKE) --silent -C $(PRINTFDIR)

# Enlace final del ejecutable
$(NAME): $(MAIN) $(OBJS) $(OUTILS) $(LIBPRINTF)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(MAIN) $(OBJS) $(OUTILS) $(LIBPRINTF) -o $(NAME) $(RFLAGS)

bonus: $(BONUS) $(LIBPRINTF) $(LIB) Makefile

$(BONUS): $(BMAIN) $(BOBJ) $(OBJS) $(LIBPRINTF)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(BMAIN) $(BOBJ) $(OBJS) $(LIBPRINTF) -o $(BONUS)

# Limpiar objetos y dependencias
clean:
	@printf "%b" "$(BLUE)Cleaning objects...$(RESET)\n"
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@$(MAKE) -C $(PRINTFDIR) clean --silent

# Limpiar todo
fclean: clean
	@printf "%b" "$(BLUE)Cleaning all files...$(RESET)\n"
	@rm -f $(NAME)
	@rm -rf $(BONUS)
	@$(MAKE) -C $(PRINTFDIR) fclean --silent

# Reconstrucción total
re: fclean all

.PHONY: all clean fclean re $(LIBPRINTF)
