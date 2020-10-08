/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/08 10:02:24 by cbussier         ###   ########lyon.fr   */
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

int		ft_get_timestamp(struct timeval start, struct timeval t)
{
	int	start_ms;
	int	t_ms;
	int	timestamp;

	// start_ms = start.tv_sec * 1000 + start.tv_usec * 0.001;
	// t_ms = t.tv_sec * 1000 + t.tv_usec * 0.001;
	// timestamp = t_ms - start_ms;
	timestamp = 1000 * (t.tv_sec - start.tv_sec) +
	(t.tv_usec - start.tv_usec) * 0.001;
	return (timestamp);
}

int		ft_get_size(int timestamp, int phi_nb, char *str)
{
	int iter;
	int size;
	int nb;

	size = 0;
	nb = timestamp;
	while (nb > 9 || nb < -9)
	{
		nb = nb / 10;
		size++;
	}
	size++;
	while (phi_nb > 9 || phi_nb < -9)
	{
		phi_nb = phi_nb / 10;
		size++;
	}
	size++;
	iter = 0;
	while (str[iter])
	{
		iter++;
		size++;
	}
	return (size);
}
