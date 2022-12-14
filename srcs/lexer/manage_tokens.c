/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:04:23 by pfrances          #+#    #+#             */
/*   Updated: 2023/01/08 16:07:45 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	get_quotes_content(t_lexer *lexer, size_t *i)
{
	char	start;

	start = lexer->input[lexer->index + *i];
	if (start != '\"' && start != '\'')
		return (true);
	(*i)++;
	while (lexer->input[lexer->index + *i] != start)
	{
		if (lexer->input[lexer->index + *i] == '\0')
		{
			if (read_new_line(lexer) == false)
				return (false);
		}
		else
			(*i)++;
	}
	(*i)++;
	return (true);
}

bool	identifier_tokens_paterns(t_lexer *lexer)
{
	size_t	i;

	i = 0;
	while (get_token_type(lexer, lexer->index + i) == TOKEN_COMMAND)
	{
		if (get_quotes_content(lexer, &i) == false)
			return (false);
		i++;
	}
	return (add_node_to_list(lexer, i));
}

bool	check_bracket(t_lexer *lexer)
{
	if (lexer->current_token_type == TOKEN_OPEN_BRACKET)
		lexer->bracket_count++;
	else if (lexer->current_token_type == TOKEN_CLOSE_BRACKET)
		lexer->bracket_count--;
	return (lexer->bracket_count >= 0);
}

bool	other_special_tokens_patterns(t_lexer *lexer)
{
	size_t	token_len;
	size_t	i;

	i = 0;
	while (lexer->tkn_types_array[i] != NULL)
	{
		token_len = ft_strlen(lexer->tkn_types_array[i]);
		if (ft_strncmp(&lexer->input[lexer->index],
				lexer->tkn_types_array[i], token_len) == 0)
		{
			if (check_bracket(lexer) == false)
				return (false);
			return (add_node_to_list(lexer, token_len));
		}
		i++;
	}
	return (false);
}

bool	manage_tokens(t_lexer *lexer)
{
	lexer->current_token_type = get_token_type(lexer, lexer->index);
	if (lexer->current_token_type == TOKEN_COMMAND)
		return (identifier_tokens_paterns(lexer));
	else
		return (other_special_tokens_patterns(lexer));
}
