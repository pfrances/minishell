/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 12:32:00 by pfrances          #+#    #+#             */
/*   Updated: 2023/02/20 11:54:49 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "ft_printf.h"
# include "libft.h"
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define BUFFER_SIZE 256
# define SPACES_CHARSET " \n\t\v\f\r"
# define TOKENS_CHARSET "&& || | ; ( )"
# define TOKENS_LEN_MAX 2
# define PROMPT_MINISHELL "Minishell > "
# define PROMPT_INDENT " > "
# define EXPORT_DECLARE_MSG "declare -x "
# define ERROR_SYNTAX_MSG "	Syntax error - unexpected token: "
# define ERROR_ALLOCATION_MSG "	Failed to allocate memory"
# define PROGRAM_STOP_MSG "exit"
# define ENVP_UNVALID_TOKEN_MSG "': not a valid identifier\n"

/******************************************************************************/
/**********************************cmd_struct**********************************/
/******************************************************************************/
typedef enum e_builtin_type
{
	NOT_BUILTIN,
	_ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	EMPTY_CMD
}	t_builtin_type;

typedef enum e_redirect_enum
{
	FILE_INPUT,
	FILE_OUTPUT,
	STD_INPUT,
	FILE_ADD_OUTPUT
}	t_redirect_enum;

typedef struct s_redirect
{
	t_redirect_enum	type;
	char			*filename;
	int				fd;
}	t_redirect;

typedef struct s_command_cnt
{
	size_t	input_output_cnt;
	size_t	args_cnt;
}	t_cmd_cnt;

typedef struct s_cmd
{
	char			*path;
	t_builtin_type	cmd_type;
	char			**args;
	t_redirect		**redirect;
	int				input_fd;
	int				input_fd_save;
	int				output_fd;
	int				output_fd_save;
}	t_cmd;
/******************************************************************************/
/**********************************ast_struct**********************************/
/******************************************************************************/
typedef enum e_token_types
{
	AND,
	OR,
	PIPE,
	SEMICOLON,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	COMMAND,
	_EOF
}	t_token_types;

typedef struct s_token
{
	t_token_types	type;
	char			*lexem;
}	t_token;

typedef struct s_ast_node
{
	t_token				*token;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_cmd				*cmd;
	bool				has_been_init;
}	t_ast_node;
/******************************************************************************/
/**********************************lexer_struct********************************/
/******************************************************************************/
typedef struct s_lexer_node
{
	t_token				token;
	struct s_lexer_node	*prev;
	struct s_lexer_node	*next;
}	t_lexer_node;

typedef struct s_lexer
{
	t_lexer_node	*list_head;
	char			*input;
	size_t			index;
	int				bracket_count;
	t_token_types	current_token_type;
	t_token			*current_token;
	t_lexer_node	*current_node;
	char			**tkn_types_array;
}	t_lexer;
/******************************************************************************/
/**********************************state_struct********************************/
/******************************************************************************/
typedef enum e_error_state_enum
{
	NO_ERROR,
	SYNTAX_ERROR,
	FILE_OPENING_FAILED,
	CMD_STOP,
	MALLOC_FAILED,
	PIPE_FAILED,
	FORK_FAILED,
	PROGRAM_STOP,
	EXIT_CALL
}	t_error_state_enum;

typedef enum e_current_phase
{
	WAITING_CMD_LINE,
	WAITING_CMD_LINE_END,
	EXECUTING_CMD,
	HERE_DOC
}	t_current_phase;

typedef struct s_envp_entry
{
	char	*name;
	char	*value;
	char	*entry;
	bool	is_declared;
}	t_envp_entry;

typedef struct s_pgrm_state
{
	t_error_state_enum	error;
	size_t				error_index;
	bool				stop_signal_flag;
	t_current_phase		current_phase;
	int					exit_status;
	char				*exit_status_str;
	t_envp_entry		**envp_entries;
	char				**envp;
}	t_pgrm_state;
/*		GLOBAL VARIABLE TO CURRENT STATE		*/
extern t_pgrm_state	g_state;

/******************************************************************************/
/********************************srcs/builtins*********************************/
/******************************************************************************/
/*				check_builtin_type.c				*/
t_builtin_type	check_builtin_type(char **cmd_args);
/*				execute_builtin.c					*/
void			execute_builtin(t_cmd *cmd);
/*				builtin_echo.c						*/
void			builtin_echo(t_cmd *cmd);
/*				builtin_cd.c						*/
void			builtin_cd(t_cmd *cmd);
/*				builtin_pwd.c						*/
void			builtin_pwd(t_cmd *cmd);
/*				builtin_export.c					*/
void			builtin_export(t_cmd *cmd);
/*				builtin_unset.c						*/
void			builtin_unset(t_cmd *cmd);
/*				builtin_env.c						*/
void			builtin_env(t_cmd *cmd);
/*				builtin_exit.c						*/
void			builtin_exit(t_cmd *cmd);
/******************************************************************************/
/********************************srcs/commands*********************************/
/******************************************************************************/
/*				execute_ast.c			*/
void			execute_ast(t_ast_node *node);
/*				execute_command.c		*/
void			execute_command(t_ast_node *cmd_node);
/*				execute_pipeline.c			*/
void			execute_pipeline(t_ast_node *node);
/*				here_doc.c				*/
void			set_here_doc(t_cmd *cmd, t_redirect *redirect);
/*				open_files.c			*/
void			open_redir_files(t_cmd *cmd);
void			open_output_files(t_cmd *cmd, t_redirect *redirect);
void			open_input_files(t_cmd *cmd, t_redirect *redirect);
/*				set_unset_redir.c		*/
void			set_redirections(t_cmd *cmd);
void			reset_redirections(t_cmd *cmd);
/*---------------------------srs/commands/init_cmd----------------------------*/
/*				allocate_cmd.c				*/
t_cmd			*allocate_cmd(char *lexem);
/*				expand_env_var.c			*/
char			*expand_env_var(char *lexem);
/*				get_command_path.c			*/
char			*get_cmd_path(char *name, char **env_paths);
/*				init_cmd.c					*/
bool			init_cmd(t_ast_node *node);
/*				set_input_output_args.c		*/
void			set_input_output_args(t_cmd *cmd, char *lexem);
/*				update_cmd_lexem.c			*/
char			*update_cmd_lexem(char *lexem, char *token,
					char *patern, size_t start);
/*				wildcards.c					*/
void			expend_wildcards(char **token);
/*				wildcards_match.c			*/
bool			wildcards_match(char *token, char *filename);
/******************************************************************************/
/********************************srcs/environment******************************/
/******************************************************************************/
/*				add_env_entry.c			*/
void			add_entry_to_env(char *name, char *value);
/*				compose_new_env_entry.c	*/
char			*compose_new_env_entry(char *env_name, char *env_value);
/*				check_envp_identifier.c	*/
bool			is_valid_envp_name(char *name);
bool			is_valid_envp_identifier(char *entry);
void			print_envp_token_error_msg(char *name, char *builtin_name);
/*				get_env_path_array.c	*/
char			**get_env_path_array(void);
/*				get_env_value.c			*/
char			*get_env_value(char *to_find);
/*				remove_env_entry.c		*/
void			remove_env_entry(char *var_name);
/*				search_entry_in_env.c			*/
t_envp_entry	*search_entry_in_env(char *var_name);
/*				set_up_envp.c			*/
bool			set_up_envp(char **envp);
/*				split_env_var.c			*/
void			split_env_var(char *var, char **var_name, char **var_value);
/*				update_all_env.c		*/
void			update_all_env(void);
/*				update_env_entry.c		*/
void			update_env_entry(char *name, char *value);
/******************************************************************************/
/**********************************srcs/lexer**********************************/
/******************************************************************************/
/*				get_cmd_line_ending.c		*/
bool			get_cmd_line_ending(t_lexer *lexer);
/*				get_next_token.c			*/
bool			get_next_token(t_lexer *lexer);
/*				get_token_type.c			*/
t_token_types	get_token_type(t_lexer *lexer, size_t index);
/*				init_lexer.c						*/
bool			init_lexer(t_lexer *lexer);
/*				lexer_list.c				*/
bool			add_node_to_list(t_lexer *lexer, size_t len);
t_lexer_node	*last_lexer_list(t_lexer_node *node);
/*				redirection_check.c			*/
bool			check_redirection(t_lexer *lexer, char *cmd, size_t len);
/*				update_bracket_count.c		*/
void			update_bracket_count(t_lexer *lexer);
/******************************************************************************/
/*********************************srcs/parser**********************************/
/******************************************************************************/
/*				create_nodes.c				*/
t_ast_node		*create_node(t_lexer *lexer);
/*				parse_nodes.c				*/
t_ast_node		*parse_semi_colon(t_ast_node *root, t_lexer *lexer);
/*				parser.c					*/
t_ast_node		*parser_job(t_lexer *lexer);
/******************************************************************************/
/*********************************srcs/tools***********************************/
/******************************************************************************/
/*				actualise_exit_status.c				*/
void			actualise_exit_status(int status);
/*				array_tools.c						*/
void			free_array(void **array);
void			**resize_array(void **array, int diff);
char			**dup_array(char **array);
size_t			array_len(void **array);
/*				atoi_with_check.c					*/
long long		ft_atol_with_error_check(const char *nptr, bool *valid_nbr);
/*				ft_split_charset.c					*/
char			**ft_split_charset(char *input, const char *charset);
/*				quotes_tools.c							*/
char			*put_in_double_quotes(char *str);
void			remove_quotes(char *cmd_token);
size_t			skip_quote_content(char *str);
/*				strjoin_with_sep.c							*/
char			*strjoin_with_sep(char *s1, char *s2, char *join);
/******************************************************************************/
/*************************************srcs*************************************/
/******************************************************************************/
/*				frees.c							*/
void			free_all(t_lexer *lexer, t_ast_node *ast_root);
void			free_cmd(t_cmd *cmd);
void			free_envp(void);
/*				init_shell.c					*/
void			init_shell(t_lexer *lexer, t_ast_node **root, char *envp[]);
/*				main.c							*/
void			print_error_msg(t_lexer *lexer);
void			actualise_exit_status(int status);
/*				signal_handling.c				*/
void			set_signal_handling(void);
#endif