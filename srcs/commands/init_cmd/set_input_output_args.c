/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_output_args.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 11:56:04 by pfrances          #+#    #+#             */
/*   Updated: 2023/02/12 10:22:13 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_token(char *str, size_t *lexem_index)
{
	size_t	i;
	size_t	j;
	char	*cmd_token;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace(str[i]) == true || str[i] == '<' || str[i] == '>')
			break ;
		if (str[i] == '"' || str[i] == '\'')
		{
			j = 1;
			while (str[i + j] != str[i])
				j++;
			i += j;
		}
		i++;
	}
	cmd_token = ft_strndup(str, i);
	if (cmd_token == NULL)
		return (NULL);
	*lexem_index += i;
	remove_quotes(cmd_token);
	return (cmd_token);
}

t_redirect	*set_input(char *lexem, size_t *i)
{
	t_redirect	*input;

	input = malloc(sizeof(t_redirect));
	if (input == NULL)
	{
		g_state.error = MALLOC_FAILED;
		return (NULL);
	}
	if (lexem[*i] == lexem[*i + 1])
		input->type = STD_INPUT;
	else
		input->type = FILE_INPUT;
	(*i) += 1 + (input->type == STD_INPUT);
	while (ft_isspace(lexem[*i]) == true)
		(*i)++;
	input->filename = get_cmd_token(&lexem[*i], i);
	if (input->filename == NULL)
	{
		free(input);
		g_state.error = MALLOC_FAILED;
		return (NULL);
	}
	return (input);
}

t_redirect	*set_output(char *lexem, size_t *i)
{
	t_redirect	*output;

	output = malloc(sizeof(t_redirect));
	if (output == NULL)
	{
		g_state.error = MALLOC_FAILED;
		return (NULL);
	}
	if (lexem[*i] == lexem[*i + 1])
		output->type = FILE_ADD_OUTPUT;
	else
		output->type = FILE_OUTPUT;
	(*i) += 1 + (output->type == FILE_ADD_OUTPUT);
	while (ft_isspace(lexem[*i]) == true)
		(*i)++;
	output->filename = get_cmd_token(&lexem[*i], i);
	if (output->filename == NULL)
	{
		free(output);
		g_state.error = MALLOC_FAILED;
		return (NULL);
	}
	return (output);
}

void	set_input_output_args(t_cmd *cmd, char *lexem)
{
	size_t		i;
	size_t		input_output_cnt;
	size_t		args_cnt;

	input_output_cnt = 0;
	args_cnt = 0;
	i = 0;
	while (g_state.error == NO_ERROR && lexem[i] != '\0')
	{
		while (ft_isspace(lexem[i]) == true)
			i++;
		if (lexem[i] == '\0')
			break ;
		if (lexem[i] == '<')
			cmd->redirect[input_output_cnt++] = set_input(lexem, &i);
		else if (lexem[i] == '>')
			cmd->redirect[input_output_cnt++] = set_output(lexem, &i);
		else
			cmd->args[args_cnt++] = get_cmd_token(&lexem[i], &i);
	}
	cmd->redirect[input_output_cnt] = NULL;
	cmd->args[args_cnt] = NULL;
}
