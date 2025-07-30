

#include "pipex.h"

void	error_exit(const char *message)
{
	if (errno != 0)
		perror(message);
	else
		write(2, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}

void	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	cleanup_and_exit(char **commands, char *error_msg)
{
	if (commands)
		free_array(commands);
	error_exit(error_msg);
	exit(EXIT_FAILURE);
}

void free_array(char **array)
{
    int i;
    
    if (!array)
        return;
        
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        error_exit(ERR_MALLOC);
    return ptr;
}

char *safe_strdup(char *s)
{
    char *dup = ft_strdup(s);
    if (!dup)
        error_exit(ERR_MALLOC);
    return dup;
}

void child_error_exit(char *cmd, char **args, char *error_msg)
{
    error_msg(error_msg);
    if (args)
        free_array(args);
    if (cmd)
        free(cmd);
    exit(127);
}

void parent_error_cleanup(int *pipefd, pid_t pid1, pid_t pid2, char *msg)
{
    if (pipefd[0] != -1) close(pipefd[0]);
    if (pipefd[1] != -1) close(pipefd[1]);
    if (pid1 > 0) kill(pid1, SIGTERM);
    if (pid2 > 0) kill(pid2, SIGTERM);
    error_exit(msg);
}