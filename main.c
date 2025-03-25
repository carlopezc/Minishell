
<<<<<<< HEAD:main.c
#include "header/ft_minishell.h"
=======
#include "header/minishell.h"


int     choose_type(char *input)
{
    
}

void    tokenization(char *input)
{
    t_minishell *minishell;
    t_token     *token;
    t_list      *token_list;

    if (!input)
        return ;

    minishell = malloc(sizeof(t_minishell));
    if (!minishell)
        return ;
    minishell->tokens = NULL;
    token_list = malloc(sizeof(t_list));
    if (!token_list)
        return ;
    token_list->content = malloc(sizeof(t_token));
    if (!token_list->content)
        return ;
    token_list->next = NULL;
    token = malloc(sizeof(t_token));
    if (!token)
        return ;
    token->str = input;
    token->type = choose_type(input);
    token_list->content = token;
    return ;
}
>>>>>>> 9caff8f667161ceb62017a70834403d6e5342168:tokenization.c

int main(void)
{
    char    *input;

    input = readline("minishell> ");
    while (*input)
    {
        add_history(input);

        ft_tokenization(input);
        free(input);
        input = readline("minishell> ");
    }
    if (input)
        free(input);
    if (!input)       
        printf("exit\n");
    //tokenization(input);
    return (0);
}

