/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin_type.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 20:11:19 by pfrances          #+#    #+#             */
/*   Updated: 2023/02/10 10:36:46 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_name_to_lower(char *cmd)
{
	char	*name;
	size_t	i;

	name = ft_strdup(cmd);
	if (name == NULL)
		return (NULL);
	i = 0;
	while (name[i] != '\0')
	{
		name[i] = ft_tolower(name[i]);
		i++;
	}
	return (name);
}

t_builtin_type	get_builtin_type(char *name, size_t len)
{
	if (ft_strncmp("echo", name, len + 1) == 0)
		return (_ECHO);
	else if (ft_strncmp("cd", name, len + 1) == 0)
		return (CD);
	else if (ft_strncmp("pwd", name, len + 1) == 0)
		return (PWD);
	else if (ft_strncmp("export", name, len + 1) == 0)
		return (EXPORT);
	else if (ft_strncmp("unset", name, len + 1) == 0)
		return (UNSET);
	else if (ft_strncmp("env", name, len + 1) == 0)
		return (ENV);
	else if (ft_strncmp("exit", name, len + 1) == 0)
		return (EXIT);
	else
		return (NOT_BUILTIN);
}

t_builtin_type	check_builtin_type(char **cmd_args)
{
	char			*name;
	size_t			len;
	t_builtin_type	type;

	name = set_name_to_lower(cmd_args[0]);
	if (name == NULL)
	{
		g_state.error = MALLOC_FAILED;
		return (NOT_BUILTIN);
	}
	len = ft_strlen(name);
	type = get_builtin_type(name, len);
	free(name);
	return (type);
}
