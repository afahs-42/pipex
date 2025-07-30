
#include "pipex_bonus.h"

void	close_all_pipes(t_pipex *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

void	first_process(t_pipex *data, int cmd_index)
{
	int	infile_fd;
	int	cmd_arg_index;

	if (data->here_doc)
	{
		infile_fd = create_here_doc(data->argv[2]);
		cmd_arg_index = 3;
	}
	else
	{
		infile_fd = open_file(data->argv[1], O_RDONLY, 0);
		cmd_arg_index = 2;
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(data->pipes[0][1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(infile_fd);
	close_all_pipes(data);
	execute_command(data->argv[cmd_arg_index], data->envp);
}

void	middle_process(t_pipex *data, int cmd_index)
{
	int	cmd_arg_index;

	if (data->here_doc)
		cmd_arg_index = cmd_index + 2;
	else
		cmd_arg_index = cmd_index + 1;
	if (dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(data->pipes[cmd_index][1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close_all_pipes(data);
	execute_command(data->argv[cmd_arg_index], data->envp);
}

void	last_process(t_pipex *data, int cmd_index)
{
	int	outfile_fd;
	int	cmd_arg_index;
	int	flags;

	if (data->here_doc)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		cmd_arg_index = cmd_index + 2;
	}
	else
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		cmd_arg_index = cmd_index + 1;
	}
	outfile_fd = open_file(data->argv[data->argc - 1], flags, 0644);
	if (dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(outfile_fd);
	close_all_pipes(data);
	execute_command(data->argv[cmd_arg_index], data->envp);
}