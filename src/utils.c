/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:50:53 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 16:32:06 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	exit(1);
}

int	open_file(char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		error_exit("Error opening file");
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

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	char	*temp;
	int		i;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	path_env = get_env_path(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK) == 0)
		{
			cleanup_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
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
		error_exit("Invalid command");
	cmd_path = find_command_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		cleanup_array(cmd_args);
		error_exit("Command not found");
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		free(cmd_path);
		cleanup_array(cmd_args);
		error_exit("Command execution failed");
	}
}
