/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 11:37:04 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int		ft_atoi(const char *str)
{
	int	i;
	int res;

	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	res = 0;
	while (str[i] > 47 && str[i] < 58)
	{
		res = res * 10;
		res = res + str[i] - 48;
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (res);
}

void	ft_itoa(char *id, int nb)
{
	int len;
	int cpy;

	len = 0;
	cpy = nb;
	id[0] = ' ';
	len++;
	if (nb == 0)
		id[1] = '0';
	while (cpy != 0)
	{
		cpy = cpy / 10;
		len++;
	}
	while (nb != 0)
	{
		id[--len] = (nb % 10) + '0';
		nb = nb / 10;
	}
}

int		get_timestamp(struct timeval s, struct timeval t)
{
	return (1000 * (t.tv_sec - s.tv_sec) + (t.tv_usec - s.tv_usec) * 0.001);
}
