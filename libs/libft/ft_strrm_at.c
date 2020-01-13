/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrm_at.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucmarti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 11:59:26 by lucmarti          #+#    #+#             */
/*   Updated: 2019/07/22 09:39:56 by lucmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy(char *dst, char *src, int i, int l)
{
	while (i < l)
	{
		dst[i] = src[i + 1];
		++i;
	}
}

char		*ft_strrm_at(char *src, int at)
{
	char	*out;
	int		i;
	int		l;

	at = at < 0 ? 0 : at;
	i = 0;
	l = ft_strlen(src);
	if (!(out = malloc(sizeof(char) * (l))))
		return (NULL);
	out[l - 1] = '\0';
	while (src[i] != '\0' && i < at)
	{
		out[i] = src[i];
		++i;
	}
	if (i == at)
	{
		out[i] = src[i + 1];
		++i;
	}
	copy(out, src, i, l);
	ft_memdel((void **)&src);
	return (out);
}
