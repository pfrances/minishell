/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 18:08:56 by pfrances          #+#    #+#             */
/*   Updated: 2023/01/07 16:23:05 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer_node	*last_lexer_list(t_lexer_node *node)
{
	if (node == NULL)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}

t_lexer_node	*add_back_lexer_list(t_lexer_node *head, t_lexer_node *node)
{
	t_lexer_node	*last;

	if (node == NULL)
		return (NULL);
	node->next = NULL;
	last = last_lexer_list(head);
	if (last == NULL)
		return (node);
	last->next = node;
	return (head);
}

t_lexer_node	*create_new_lexer_node(t_token_types type, char *content)
{
	t_lexer_node	*node;

	node = malloc(sizeof(t_lexer_node));
	if (node == NULL)
		return (NULL);
	node->token.lexem = content;
	node->token.type = type;
	node->next = NULL;
	return (node);
}

bool	add_node_to_list(t_lexer *lexer, size_t len)
{
	t_lexer_node	*new_node;
	char			*token_content;

	if (len == 0)
		return (true);
	token_content = ft_strndup(&lexer->input[lexer->index], len);
	if (token_content == NULL)
		return (false);
	lexer->current_token_type = get_token_type(lexer, lexer->index);
	new_node = create_new_lexer_node(lexer->current_token_type, token_content);
	lexer->list = add_back_lexer_list(lexer->list, new_node);
	lexer->index += len;
	return (lexer->list != NULL);
}
