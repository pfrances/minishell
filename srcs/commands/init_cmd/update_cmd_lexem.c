/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_cmd_lexem.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:58:09 by pfrances          #+#    #+#             */
/*   Updated: 2023/02/10 10:36:46 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*update_cmd_lexem(char *lexem, char *token, char *patern, size_t start)
{
	char	*result;
	size_t	lexem_len;
	size_t	token_len;
	size_t	patern_len;

	lexem_len = ft_strlen(lexem);
	token_len = ft_strlen(token);
	if (patern != NULL)
		patern_len = ft_strlen(patern);
	else
		patern_len = 0;
	result = malloc(sizeof(char) * (lexem_len - token_len + patern_len + 1));
	if (result == NULL)
	{
		g_state.error = MALLOC_FAILED;
		free(lexem);
		return (NULL);
	}
	ft_strlcpy(result, lexem, start + 1);
	if (patern != NULL)
		ft_strlcat(result, patern, ft_strlen(result) + patern_len + 1);
	ft_strlcat(result, lexem + start + token_len,
		lexem_len + patern_len - token_len + 1);
	free(lexem);
	return (result);
}
