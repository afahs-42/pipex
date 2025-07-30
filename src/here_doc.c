/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 20:00:00 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_next_line(void)
{
	char	*line;
	char	buffer;
	int		i;
	int		bytes_read;

	line = malloc(1000);
	if (!line)
		return (NULL);
	i = 0;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, &buffer, 1);
		if (bytes_read <= 0)
			break;
		if (buffer == '\n')
		{
			line[i] = '\0';
			return (line);
		}
		line[i] = buffer;
		i++;
		if (i >= 999)
			break;
	}
	line[i] = '\0';
	if (i == 0 && bytes_read <= 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	create_here_doc(char *limiter)
{
	int		pipe_fd[2];
	char	*line;
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		error_exit("Failed to create here_doc pipe", 1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		error_exit("Failed to fork for here_doc", 1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write(STDOUT_FILENO, "pipe heredoc> ", 14);
		while (1)
		{
			line = get_next_line();
			if (!line || ft_strcmp(line, limiter) == 0)
			{
				if (line)
					free(line);
				break;
			}
			write(pipe_fd[1], line, ft_strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
			write(STDOUT_FILENO, "pipe heredoc> ", 14);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	return (pipe_fd[0]);
}