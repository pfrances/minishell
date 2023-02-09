/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:32:19 by pfrances          #+#    #+#             */
/*   Updated: 2023/02/09 20:54:28 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_msg(t_lexer *lexer)
{
	if (g_state.error_state == SYNTAX_ERROR)
	{
		ft_putstr_fd(ERROR_SYNTAX_MSG, STDERR_FILENO);
		write(STDERR_FILENO, lexer->input, g_state.error_index);
		write(STDERR_FILENO, "==>", 3);
		ft_putendl_fd(lexer->input + g_state.error_index, STDERR_FILENO);
	}
	else if (g_state.error_state == ALLOCATION_FAILED)
		ft_putendl_fd(ERROR_ALLOCATION_MSG, STDERR_FILENO);
	else if (g_state.error_state == ENV_ERROR)
		ft_putendl_fd(ENV_ERROR_MSG, STDERR_FILENO);
	else if (g_state.error_state == PROGRAM_STOP)
		ft_putendl_fd(PROGRAM_STOP_MSG, STDERR_FILENO);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_lexer		lexer;
	t_ast_node	*root;

	(void)argv;
	if (argc == 1)
	{
		shell_init(&lexer, &root, envp);
		while (1)
		{
			root = parser_job(&lexer);
			if (g_state.error_state != NO_ERROR)
				print_error_msg(&lexer);
			else
				execute_ast(root);
			free_all(&lexer, root);
			if (g_state.error_state >= ALLOCATION_FAILED)
				return (g_state.exit_status);
		}
	}
	return (0);
}
