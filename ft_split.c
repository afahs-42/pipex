/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:28:59 by afahs             #+#    #+#             */
/*   Updated: 2025/07/11 10:39:35 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	sep(char c, char s)
{
	if (c == s || c == '\0')
		return (1);
	return (0);
}

static void	write_word(char *dest, char const *from, char c)
{
	int	i;

	i = 0;
	while (!sep(from[i], c))
	{
		dest[i] = from[i];
		i++;
	}
	dest[i] = '\0';
}

static void	*create_word(char **split, int word, int j)
{
	split[word] = (char *)malloc(sizeof(char) * (j + 1));
	if (!split[word])
	{
		while (word > 0)
			free(split[--word]);
		return (NULL);
	}
	return ((void *)1);
}

static void	*write_split(char **split, char const *str, char c)
{
	int		i;
	int		j;
	int		word;

	word = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			i++;
		else
		{
			j = 0;
			while (!sep(str[i + j], c))
				j++;
			if (!create_word(split, word, j))
				return (NULL);
			write_word(split[word], str + i, c);
			i += j;
			word++;
		}
	}
	return ((void *)1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	char	*str;
	int		words;
	int		i;

	if (!s)
		return (NULL);
	str = (char *)s;
	i = 0;
	words = 0;
	while (s[i])
	{
		if (!sep(s[i], c) && sep(s[i + 1], c))
			words++;
		i++;
	}
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[words] = 0;
	if (!write_split(res, str, c))
		return (NULL);
	return (res);
}
