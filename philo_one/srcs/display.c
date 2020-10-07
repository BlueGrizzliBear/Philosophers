/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 18:56:10 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

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

int		ft_secure_msg(t_phi *phi, char *msg)
{
	if (pthread_mutex_lock(phi->params->display))
		return (ft_error(ERROR_LOCK_MUTEX));
	write(1, msg, ft_strlen(msg));
	if (pthread_mutex_unlock(phi->params->display))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		ft_build_msg(t_phi *phi, int timestamp, char *str, int size)
{
	char	msg[size];
	char	*m;

	m = memset((void*)msg, '\0', size);
	ft_fill_msg_nb(m, timestamp);
	ft_fill_msg_nb(m, phi->id);
	ft_fill_msg_str(m, str);
	ft_secure_msg(phi, m);
	return (0);
}

int		ft_display(t_phi *phi, char *str)
{
	struct timeval	now;
	int				timestamp;
	int				size;
	static int		reaper = 0;

	if (pthread_mutex_lock(phi->params->available))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (reaper != 0)
	{
		if (pthread_mutex_unlock(phi->params->available))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (-1);
	}
	if (phi->status == 0)
		reaper += 1;
	if (pthread_mutex_unlock(phi->params->available))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	timestamp = ft_get_timestamp(phi->params->start, now);
	size = (ft_get_size(timestamp, phi->id, str) + 3);
	ft_build_msg(phi, timestamp, str, size);
	return (0);
}
