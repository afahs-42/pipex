/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:18:07 by afahs             #+#    #+#             */
/*   Updated: 2025/07/11 10:18:08 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_args(int argc, char **argv)
{
    if (argc != 5)
    {
        write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 36);
        exit(EXIT_FAILURE);
    }
    if (!argv[1] || !argv[1][0])
        error_exit("Error: Input file is missing\n");
    if (!argv[2] || !argv[2][0])
        error_exit("Error: Command 1 is missing\n");
    if (!argv[3] || !argv[3][0])
        error_exit("Error: Command 2 is missing\n");
    if (!argv[4] || !argv[4][0])
        error_exit("Error: Output file is missing\n");
    validate_file_access(argv[1], argv[4]);
}

void validate_file_access(char *input_file, char *output_file)
{
	char *last_slash;
    if (access(input_file, F_OK) != 0)
        perror("Error accessing input file");
	else if (access(input_file, R_OK) != 0)
		perror("Error reading input file");
	last_slash = ft_strrchr(output_file, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(output_file, W_OK) != 0)
			write(2, "Error: Cannot write to output file\n", 35);
		*last_slash = '/';
	}
}

void	validate_arguments(int argc, char **argv)
{
	if (argc != 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 file2");
	if (access(argv[1], F_OK) == -1)
		error_exit("Input file does not exist");
	if (access(argv[1], R_OK) == -1)
		error_exit("Input file is not readable");
}

void	child_process(char **argv, char **envp, int *pipe_fd)
{
	int	infile_fd;

	infile_fd = open_file(argv[1], O_RDONLY, 0);
	close(pipe_fd[0]);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("Failed to redirect input");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output");
	close(infile_fd);
	close(pipe_fd[1]);
	execute_command(argv[2], envp);
}

void	parent_process(char **argv, char **envp, int *pipe_fd)
{
	int	outfile_fd;

	outfile_fd = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("Failed to redirect input");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("Failed to redirect output");
	close(pipe_fd[0]);
	close(outfile_fd);
	execute_command(argv[3], envp);
}

void	execute_pipex(char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		error_exit("Failed to create pipe");
	pid = fork();
	if (pid == -1)
		error_exit("Failed to fork process");
	if (pid == 0)
		child_process(argv, envp, pipe_fd);
	else
	{
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		parent_process(argv, envp, pipe_fd);
	}
}

int	main(int argc, char **argv, char **envp)
{
	validate_arguments(argc, argv);
	execute_pipex(argv, envp);
	return (0);
}