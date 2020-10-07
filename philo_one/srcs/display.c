/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 13:36:31 by cbussier         ###   ########lyon.fr   */
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

int		ft_build_msg(t_phi *phi, char *str)
{
	struct timeval	now;
	int				timestamp;
	char			*msg;
	int				size;

	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	timestamp = ft_get_timestamp(phi->params->start, now);
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

	// if (pthread_mutex_lock(phi->params->display))
	// 	return (ft_error(ERROR_LOCK_MUTEX));
	if (reaper != 0)
	{
		// if (pthread_mutex_unlock(phi->params->display))
		// 	return (ft_error(ERROR_UNLOCK_MUTEX));
		return (-1);
	}
	if (phi->status == 0)
		reaper += 1;
	if (ft_build_msg(phi, str))
		return (1);
	// if (pthread_mutex_unlock(phi->params->display))
	// 	return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		fts_display(t_phi *phi, char *str)
{
	int ret;

	ret = 0;
	if (pthread_mutex_lock(phi->params->display))
		return (ft_error(ERROR_LOCK_MUTEX));
	if ((ret = ft_display(phi, str)))
	{
		if (pthread_mutex_unlock(phi->params->display))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	}
	if (pthread_mutex_unlock(phi->params->display))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}