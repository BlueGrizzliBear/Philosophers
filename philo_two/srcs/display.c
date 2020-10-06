/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/06 22:03:27 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

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

void	ft_fill_msg_str(char *msg, char *str)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (msg[len])
		len++;
	while (str[i])
	{
		msg[len + i] = str[i];
		i++;
	}
	msg[len + i] = '\0';
}

void	ft_fill_msg_nb(char *msg, int nb)
{
	int size;
	int a;
	int len;

	len = 0;
	while (msg[len])
		len++;
	size = 0;
	a = nb;
	while (a > 9)
	{
		a = a / 10;
		size++;
	}
	size++;
	msg[len + size] = ' ';
	size--;
	while (nb > 9)
	{
		msg[len + size] = nb % 10 + '0';
		nb = nb / 10;
		size--;
	}
	msg[len + size] = nb % 10 + '0';
}

int		ft_build_msg(t_phi *phi, char *str)
{
	struct timeval	now;
	int				timestamp;
	char			*msg;
	int				size;

	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	timestamp = ft_get_timestamp(phi->start, now);
	size = (ft_get_size(timestamp, phi->id, str) + 3);
	if (!(msg = malloc(sizeof(char) * size)))
		return (ft_error(ERROR_MEM_ALLOC));
	msg = memset(msg, '\0', size);
	ft_fill_msg_nb(msg, timestamp);
	ft_fill_msg_nb(msg, phi->id);
	ft_fill_msg_str(msg, str);
	ft_putstr(msg);
	free(msg);
	msg = NULL;
	return (0);
}

int		ft_display(t_phi *phi, char *str)
{
	static int	reaper = 0;

	if (sem_wait(phi->params->display))
		return (ft_error(ERROR_LOCK_SEM));
	if (reaper != 0)
	{
		if (sem_post(phi->params->display))
			return (ft_error(ERROR_UNLOCK_SEM));
		return (-1);
	}
	if (phi->status == 0)
		reaper += 1;
	if (ft_build_msg(phi, str))
		return (1);
	if (sem_post(phi->params->display))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}
