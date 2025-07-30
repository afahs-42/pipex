/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:18:15 by afahs             #+#    #+#             */
/*   Updated: 2025/07/30 20:00:00 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>

/* Main execution functions */
void	execute_pipex(int argc, char **argv, char **envp);
void	setup_pipes(int ***pipes, int cmd_count);
void	execute_commands(int argc, char **argv, char **envp, int **pipes);
void	cleanup_pipes(int **pipes, int cmd_count);

/* Process functions */
void	first_process(char **argv, char **envp, int **pipes, int here_doc, int cmd_count);
void	middle_process(char **argv, char **envp, int **pipes, int cmd_index, int here_doc, int cmd_count);
void	last_process(char **argv, char **envp, int **pipes, int cmd_index, int here_doc, int argc, int cmd_count);

/* Here_doc functions */
int		create_here_doc(char *limiter);

/* Utility functions */
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strdup(const char *s1);

/* Command and path handling */
char	*find_command_path(char *cmd, char **envp, int i);
void	execute_command(char *cmd_str, char **envp);

/* Error handling */
void	error_exit(char *message, int i);
void	cleanup_array(char **arr);

/* File operations */
int		open_file(char *filename, int flags, int mode);

/* Helper functions */
int		is_here_doc(char *arg);
int		get_cmd_count(int argc, int here_doc);
int		get_cmd_arg_index(int cmd_index, int here_doc);

#endif