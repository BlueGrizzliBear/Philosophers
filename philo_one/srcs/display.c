/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/20 09:36:57 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_fill_msg_str(char *msg, char *id, char *str, int size)
{
	int i;

	i = -1;
	while (id[++i])
		msg[size++] = id[i];
	i = -1;
	while (str[++i])
		msg[size++] = str[i];
	msg[size + i] = '\0';
	return (size);
}

int		ft_fill_msg_nb(char *msg, int nb)
{
	int size;
	int cpy;

	size = 0;
	cpy = nb;
	while (cpy > 9)
	{
		cpy = cpy / 10;
		size++;
	}
	cpy = size;
	while (nb > 9)
	{
		msg[size--] = nb % 10 + '0';
		nb = nb / 10;
	}
	msg[size] = nb % 10 + '0';
	cpy++;
	return (cpy);
}

int		ft_build_msg(t_phi *phi, int timestamp, char *str)
{
	char	msg[64];
	int		size;

	size = ft_fill_msg_nb(msg, timestamp);
	size = ft_fill_msg_str(msg, phi->id, str, size);
	if (pthread_mutex_lock(phi->params->display))
		return (ft_error(ERROR_LOCK_MUTEX));
	write(1, (void*)msg, size);
	if (pthread_mutex_unlock(phi->params->display))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		ft_display(t_phi *phi, char *str)
{
	struct timeval	now;
	// static int		reaper = 0;

	if (ft_is_over(phi))
		return (-1);
	// if (pthread_mutex_lock(phi->params->game_status))
	// 	return (ft_error(ERROR_LOCK_MUTEX));
	// if (phi->params->game == 0)
	// {
	// 	if (pthread_mutex_unlock(phi->params->game_status))
	// 		return (ft_error(ERROR_UNLOCK_MUTEX));
	// 	return (-1);
	// }
	// if (pthread_mutex_unlock(phi->params->game_status))
	// 	return (ft_error(ERROR_UNLOCK_MUTEX));
	// if (reaper != 0)
	// 	return (-1);
	// if (phi->status == 0)
	// 	reaper += 1;
	gettimeofday(&now, NULL);
	ft_build_msg(phi, ft_get_timestamp(phi->params->start, now), str);
	return (0);
}
