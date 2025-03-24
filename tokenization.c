
#include "header/minishell.h"

void    tokenization(char *input)
{
    (void)input;
    return ;
    

}

int main(void)
{
    char    *input;

    input = readline("minishell> ");
    while (*input)
    {
        printf("%s\n", input);
        add_history(input);
        tokenization(input);
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

