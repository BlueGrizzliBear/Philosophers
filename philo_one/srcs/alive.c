/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/01 23:08:56 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_is_dead(t_phi *phi)
{
	static struct timeval now;

	gettimeofday(&now, NULL);
	if (get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
		return (1);
	return (0);
}

void	*th_brain(void *arg)
{
	t_phi	*phi;

	phi = (t_phi*)(arg);
	while (phi->status == 1 && phi->params->game == 1)
	{
		if (pthread_mutex_lock(phi->check) && ft_error(ERROR_LOCK_MUTEX))
			return ((void*)0);
		if (phi->has_eaten != phi->params->must_eat && ft_is_dead(phi))
		{
			phi->status = 0;
			ft_display(phi, " died\n", 1);
			// phi->params->game = 0;
			if (pthread_mutex_unlock(phi->check) ||
			pthread_mutex_unlock(phi->params->game_over))
			{
				ft_error(ERROR_UNLOCK_MUTEX);
				return ((void*)0);
			}
			return ((void*)0);
		}
		if (pthread_mutex_unlock(phi->check) && ft_error(ERROR_UNLOCK_MUTEX))
			return ((void*)0);
		usleep(5000);
	}
	return ((void*)0);
}

int		ft_sleep(t_phi *phi)
{
	if (ft_display(phi, " is sleeping\n", 0))
		return (-1);
	if (ft_standby(phi->params->time_to_sleep))
		return (-1);
	return (0);
}

int		ft_think(t_phi *phi)
{
	if (ft_display(phi, " is thinking\n", 0))
		return (-1);
	return (0);
}

void	*th_is_alive(void *arg)
{
	t_phi		*phi;

	phi = (t_phi *)arg;
	if (pthread_create(&phi->brain, NULL, &th_brain, phi))
	{
		ft_error(ERROR_CREATE_THREAD);
		return ((void*)0);
	}
	while (1)
	{
		if (ft_eat(phi))
			break ;
		if (ft_sleep(phi))
			break ;
		if (ft_think(phi))
			break ;
	}
	phi->status = 0;
	pthread_join(phi->brain, NULL);
	if (pthread_mutex_unlock(phi->params->display))
	// if (pthread_mutex_unlock(phi->params->display) ||
	// pthread_mutex_unlock(phi->order) ||
	// pthread_mutex_unlock(phi->left_fork->mutex) ||
	// pthread_mutex_unlock(phi->right_fork->mutex))
		ft_error(ERROR_UNLOCK_MUTEX);
	return ((void*)0);
}
