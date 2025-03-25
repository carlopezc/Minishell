
#include "header/ft_minishell.h"

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

