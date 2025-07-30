
#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		here_doc;
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
}	t_pipex;

/* Main execution functions */
void	execute_pipex_bonus(t_pipex *data);
void	setup_pipes(t_pipex *data);
void	execute_commands(t_pipex *data);
void	cleanup_pipes(t_pipex *data);

/* Process functions */
void	first_process(t_pipex *data, int cmd_index);
void	middle_process(t_pipex *data, int cmd_index);
void	last_process(t_pipex *data, int cmd_index);

/* Here_doc functions */
void	handle_here_doc(t_pipex *data);
int		create_here_doc(char *limiter);

/* Utility functions */
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
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

/* Parsing functions */
void	parse_arguments(t_pipex *data);
void	init_pipex(t_pipex *data, int argc, char **argv, char **envp);

#endif