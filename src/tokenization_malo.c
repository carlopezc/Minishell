
#include "../header/ft_minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

t_minishell	*ft_init_struct(void)
{
	t_minishell *minishell;
    	t_list      *token_list;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	token_list = malloc(sizeof(t_list));
	if (!token_list)
		return (NULL);
	minishell->tokens = token_list;
	token_list->content = malloc(sizeof(t_token));
	if (!token_list->content)
		return (NULL);
	token_list->next = NULL;
	token_list->content = NULL;
	return (minishell);
}
/*
void	ft_create_node(t_minishell *minishell, char *input)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->str = input;
	token->type = 0;
	//ft_lstadd_back(&(minishell->tokens->content));
	return ;
}
*/
int	ft_is_operator(char *input, t_minishell *minishell)
{
	(void)minishell;
	if (!ft_strncmp(input, "|", ft_strlen(input)))
	{
		ft_printf("SUB-TYPE: pipe\n");
		return (1);
	}
	else if (!ft_strncmp(input, "<", ft_strlen(input)))
	{
		ft_printf("SUB-TYPE: input redirection\n");
		return (1);
	}
	else if (!ft_strncmp(input, ">", ft_strlen(input)))
	{
		ft_printf("TYPE: output redirection\n");
		return (1);
	}
	else if (!ft_strncmp(input, "<<", ft_strlen(input)))
	{
		ft_printf("TYPE: heredoc\n");
		return (1);
	}
	else if (!ft_strncmp(input, ">>", ft_strlen(input)))
	{
		ft_printf("TYPE: output + append redirection\n");
		return (1);
	}
	else
		return (0);
}

int	ft_is_builtin(char *input, t_minishell *minishell)
{
	(void)minishell;
	if (!ft_strncmp(input, "echo", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "cd", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "pwd", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "export", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "unset", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "env", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "exit", ft_strlen(input)))
		return (1);
	else
		return (0);
}

int	ft_check_var(char **input, t_minishell *minishell)
{
	int	i;

	(void)minishell;
	i = 0;
	if (input && *input && (*input)[i] == '$')
	{
		i++;
		if (*input && (*input)[i] && (ft_isalpha((*input)[i]) || (*input)[i] == '_'))
		{
			i++;
			while (*input && (*input)[i] && (ft_isalpha((*input)[i])
					|| ft_isdigit((*input)[i]) || (*input)[i] == '_'))
				i++;
		}
		else
			return (ft_printf("Error en nombre de variable\n"), 0);
		if (!(*input)[i])
			return (1);
	}
	return (0);
}

int	ft_check_quotes(char **input, t_minishell *minishell)
{
	int	flag_check;
	int	i;

	(void)minishell;
	i = 0;
	flag_check = 0;
	while ((*input)[i])
	{
		if ((*input)[0] == '\'')
			flag_check = 1;
		else if ((*input)[0] == '"')
			flag_check = 2;
		//2 porque uno del caracter y otro del \n
		if (((*input)[i] == '\'') && (flag_check == 1) && ((size_t)i == ft_strlen(*input) - 1))
		{
			*input = ft_substr(*input, 1, ft_strlen(*input) - 2);
			return (1);
		}
		else if (((*input)[i] == '"') && (flag_check == 2) && ((size_t)i == ft_strlen(*input) - 1))
		{
			*input = ft_substr(*input, 1, ft_strlen(*input) - 2);
			return (1);
		}
		i++;
	}
	if (flag_check == 1 || flag_check == 2)
		ft_printf("Error: Comillas sin cerrar!!!!!!\n");
	return (0);
}

void	ft_check_type(char *input, t_minishell *minishell)
{
	if (ft_is_builtin(input, minishell))
	{
		ft_printf("TYPE: built-in\n");
		ft_printf("INPUT: %s\n", input);
		return ;
	}
	if (ft_is_operator(input, minishell))
	{
		ft_printf("TYPE: operator\n");
		ft_printf("INPUT: %s\n", input);
		return ;
	}
	if (ft_check_var(&input, minishell))
	{
		ft_printf("TYPE: environment variable\n");
		ft_printf("INPUT: %s\n", input);
	}
	ft_check_quotes(&input, minishell);
	ft_printf("FINAL INPUT: %s\n", input);
	return ;
}


void    ft_tokenization(char **s_input, t_minishell *minishell)
{
	int	i;

	i = 0;
	if (!s_input || !*s_input)
		return ;
	while (s_input && s_input[i])
	{
		ft_check_type(s_input[i], minishell);
		i++;
	}
    	return ;
}
