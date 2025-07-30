/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:18:07 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 18:47:09 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(char **argv, char **envp, int *pipe_fd)
{
	int	infile_fd;

	close(pipe_fd[0]);
	infile_fd = open_file(argv[1], O_RDONLY, 0);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(infile_fd);
	close(pipe_fd[1]);
	execute_command(argv[2], envp);
}

void	parent_process(char **argv, char **envp, int *pipe_fd)
{
	int	outfile_fd;

	close(pipe_fd[1]);
	outfile_fd = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input", 1);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output", 1);
	close(pipe_fd[0]);
	close(outfile_fd);
	execute_command(argv[3], envp);
}

void	execute_pipex(char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipe_fd) == -1)
		error_exit("Failed to create pipe", 1);
	pid1 = fork();
	if (pid1 == -1)
		error_exit("Failed to fork process", 1);
	if (pid1 == 0)
		child_process(argv, envp, pipe_fd);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("Failed to fork process", 1);
	if (pid2 == 0)
		parent_process(argv, envp, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	exit(WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 file2", 1);
	execute_pipex(argv, envp);
	if (access(argv[1], F_OK) == -1)
		error_exit("Input file does not exist", 1);
	if (access(argv[1], R_OK) == -1)
		error_exit("Input file is not readable", 1);
	return (0);
}
