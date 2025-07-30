/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:50:53 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 18:55:50 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *message, int i)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	exit(i);
}

int	open_file(char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		error_exit("Error opening file", 1);
	return (fd);
}

char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

// chars[0] = path_env / chars[1] = full_path / chars[2] = temp
char	*find_command_path(char *cmd, char **envp, int i)
{
	char	**paths;
	char	*chars[3];

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	chars[0] = get_env_path(envp);
	if (!chars[0])
		return (NULL);
	paths = ft_split(chars[0], ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		chars[2] = ft_strjoin(paths[i++], "/");
		chars[1] = ft_strjoin(chars[2], cmd);
		free(chars[2]);
		if (access(chars[1], F_OK) == 0)
		{
			cleanup_array(paths);
			return (chars[1]);
		}
		free(chars[1]);
	}
	cleanup_array(paths);
	return (NULL);
}

void	execute_command(char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd_str, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		cleanup_array(cmd_args);
		error_exit("Invalid command", 127);
	}
	cmd_path = find_command_path(cmd_args[0], envp, 0);
	if (!cmd_path)
	{
		cleanup_array(cmd_args);
		error_exit("Command not found", 127);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		free(cmd_path);
		cleanup_array(cmd_args);
		error_exit("Command execution failed", 1);
	}
}
