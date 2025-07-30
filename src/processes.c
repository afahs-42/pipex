/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 20:00:00 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_all_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	first_process(char **argv, char **envp, int **pipes, int here_doc, int cmd_count)
{
	int	infile_fd;
	int	cmd_arg_index;

	if (here_doc)
	{
		infile_fd = create_here_doc(argv[2]);
		cmd_arg_index = 3;
	}
	else
	{
		infile_fd = open_file(argv[1], O_RDONLY, 0);
		cmd_arg_index = 2;
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(pipes[0][1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(infile_fd);
	close_all_pipes(pipes, cmd_count);
	execute_command(argv[cmd_arg_index], envp);
}

void	middle_process(char **argv, char **envp, int **pipes, int cmd_index, int here_doc, int cmd_count)
{
	int	cmd_arg_index;

	cmd_arg_index = get_cmd_arg_index(cmd_index, here_doc);
	if (dup2(pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(pipes[cmd_index][1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close_all_pipes(pipes, cmd_count);
	execute_command(argv[cmd_arg_index], envp);
}

void	last_process(char **argv, char **envp, int **pipes, int cmd_index, int here_doc, int argc, int cmd_count)
{
	int	outfile_fd;
	int	cmd_arg_index;
	int	flags;

	cmd_arg_index = get_cmd_arg_index(cmd_index, here_doc);
	if (here_doc)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	outfile_fd = open_file(argv[argc - 1], flags, 0644);
	if (dup2(pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(outfile_fd);
	close_all_pipes(pipes, cmd_count);
	execute_command(argv[cmd_arg_index], envp);
}