/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:18:07 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 20:00:00 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_here_doc(char *arg)
{
	return (ft_strncmp(arg, "here_doc", 8) == 0 && ft_strlen(arg) == 8);
}

int	get_cmd_count(int argc, int here_doc)
{
	if (here_doc)
		return (argc - 4);
	return (argc - 3);
}

int	get_cmd_arg_index(int cmd_index, int here_doc)
{
	if (here_doc)
		return (cmd_index + 3);
	return (cmd_index + 2);
}

void	validate_arguments(int argc, char **argv, int here_doc)
{
	if (here_doc)
	{
		if (argc < 6)
			error_exit("Usage: ./pipex here_doc LIMITER cmd1 cmd2 ... file", 1);
	}
	else
	{
		if (argc < 5)
			error_exit("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2", 1);
		if (access(argv[1], F_OK) == -1)
			error_exit("Input file does not exist", 1);
		if (access(argv[1], R_OK) == -1)
			error_exit("Input file is not readable", 1);
	}
}

void	setup_pipes(int ***pipes, int cmd_count)
{
	int	i;

	if (cmd_count < 2)
		error_exit("Need at least 2 commands", 1);
	*pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!*pipes)
		error_exit("Failed to allocate pipes", 1);
	i = 0;
	while (i < cmd_count - 1)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (!(*pipes)[i])
		{
			cleanup_pipes(*pipes, i);
			error_exit("Failed to allocate pipe", 1);
		}
		if (pipe((*pipes)[i]) == -1)
		{
			cleanup_pipes(*pipes, i + 1);
			error_exit("Failed to create pipe", 1);
		}
		i++;
	}
}

void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}

void	execute_commands(int argc, char **argv, char **envp, int **pipes)
{
	int		cmd_count;
	int		here_doc;
	pid_t	*pids;
	int		i;
	int		status;

	here_doc = is_here_doc(argv[1]);
	cmd_count = get_cmd_count(argc, here_doc);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		error_exit("Failed to allocate pids", 1);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free(pids);
			error_exit("Failed to fork process", 1);
		}
		if (pids[i] == 0)
		{
			if (i == 0)
				first_process(argv, envp, pipes, here_doc, cmd_count);
			else if (i == cmd_count - 1)
				last_process(argv, envp, pipes, i, here_doc, argc, cmd_count);
			else
				middle_process(argv, envp, pipes, i, here_doc, cmd_count);
		}
		i++;
	}
	cleanup_pipes(pipes, cmd_count);
	i = 0;
	while (i < cmd_count)
		waitpid(pids[i++], &status, 0);
	free(pids);
}

void	execute_pipex(int argc, char **argv, char **envp)
{
	int	**pipes;
	int	here_doc;
	int	cmd_count;

	here_doc = is_here_doc(argv[1]);
	validate_arguments(argc, argv, here_doc);
	cmd_count = get_cmd_count(argc, here_doc);
	setup_pipes(&pipes, cmd_count);
	execute_commands(argc, argv, envp, pipes);
}

int	main(int argc, char **argv, char **envp)
{
	execute_pipex(argc, argv, envp);
	return (0);
}