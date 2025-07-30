
#include "pipex_bonus.h"

void	init_pipex(t_pipex *data, int argc, char **argv, char **envp)
{
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	data->here_doc = 0;
	data->pipes = NULL;
	data->pids = NULL;
	parse_arguments(data);
}

void	parse_arguments(t_pipex *data)
{
	if (data->argc < 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2", 1);
	if (ft_strncmp(data->argv[1], "here_doc", 8) == 0)
	{
		if (data->argc < 6)
			error_exit("Usage: ./pipex here_doc LIMITER cmd1 cmd2 ... file", 1);
		data->here_doc = 1;
		data->cmd_count = data->argc - 4;
	}
	else
	{
		data->cmd_count = data->argc - 3;
		if (access(data->argv[1], F_OK) == -1)
			error_exit("Input file does not exist", 1);
		if (access(data->argv[1], R_OK) == -1)
			error_exit("Input file is not readable", 1);
	}
}

void	setup_pipes(t_pipex *data)
{
	int	i;

	if (data->cmd_count <= 1)
		error_exit("Need at least 2 commands", 1);
	data->pipes = malloc(sizeof(int *) * (data->cmd_count - 1));
	if (!data->pipes)
		error_exit("Failed to allocate pipes", 1);
	i = 0;
	while (i < data->cmd_count - 1)
	{
		data->pipes[i] = malloc(sizeof(int) * 2);
		if (!data->pipes[i] || pipe(data->pipes[i]) == -1)
			error_exit("Failed to create pipe", 1);
		i++;
	}
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
		error_exit("Failed to allocate pids", 1);
}

void	cleanup_pipes(t_pipex *data)
{
	int	i;

	if (data->pipes)
	{
		i = 0;
		while (i < data->cmd_count - 1)
		{
			if (data->pipes[i])
			{
				close(data->pipes[i][0]);
				close(data->pipes[i][1]);
				free(data->pipes[i]);
			}
			i++;
		}
		free(data->pipes);
	}
	if (data->pids)
		free(data->pids);
}

void	execute_commands(t_pipex *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->cmd_count)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			error_exit("Failed to fork process", 1);
		if (data->pids[i] == 0)
		{
			if (i == 0)
				first_process(data, i);
			else if (i == data->cmd_count - 1)
				last_process(data, i);
			else
				middle_process(data, i);
		}
		i++;
	}
	cleanup_pipes(data);
	i = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &status, 0);
		i++;
	}
}

void	execute_pipex_bonus(t_pipex *data)
{
	setup_pipes(data);
	execute_commands(data);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	init_pipex(&data, argc, argv, envp);
	execute_pipex_bonus(&data);
	return (0);
}