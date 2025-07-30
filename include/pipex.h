/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:18:15 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 16:39:53 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>

void	execute_pipex(char **argv, char **envp);
void	child_process(char **argv, char **envp, int *pipe_fd);
void	parent_process(char **argv, char **envp, int *pipe_fd);

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strdup(const char *s1);

char	*find_command_path(char *cmd, char **envp);
void	execute_command(char *cmd_str, char **envp);

void	error_exit(char *message);
void	cleanup_array(char **arr);

int		open_file(char *filename, int flags, int mode);
#endif