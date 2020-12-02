/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 12:02:32 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_standby(int time)
{
	struct timeval now;
	struct timeval standby_start;

	gettimeofday(&standby_start, NULL);
	gettimeofday(&now, NULL);
	while (get_timestamp(standby_start, now) < time)
		gettimeofday(&now, NULL);
	return (0);
}

int		unlock_forks(t_phi *phi)
{
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	if (pthread_mutex_unlock(phi->right_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		clean_mutexes(t_phi *phi)
{
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		ft_error(ERROR_UNLOCK_MUTEX);
	if (pthread_mutex_unlock(phi->order))
		ft_error(ERROR_UNLOCK_MUTEX);
	phi->ordo = 0;
	return (-1);
}

int		lock_forks(t_phi *phi)
{
	if (pthread_mutex_lock(phi->order))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (pthread_mutex_lock(phi->left_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (ft_display(phi, " has taken a fork\n", 0))
		return (clean_mutexes(phi));
	if (phi->params->nb < 2)
	{
		while (phi->params->game == 1)
			ft_standby(1);
		return (clean_mutexes(phi));
	}
	if (pthread_mutex_lock(phi->right_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (ft_display(phi, " has taken a fork\n", 0))
	{
		if (pthread_mutex_unlock(phi->right_fork->mutex))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (clean_mutexes(phi));
	}
	if (pthread_mutex_unlock(phi->order))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	phi->ordo = 0;
	return (0);
}

int		ft_eat(t_phi *phi)
{
	if (lock_forks(phi))
		return (-1);
	if (pthread_mutex_lock(phi->check))
		return (ft_error(ERROR_LOCK_MUTEX));
	gettimeofday(&phi->last_meal, NULL);
	if (ft_display(phi, " is eating\n", 0))
	{
		if (unlock_forks(phi))
			return (1);
		if (pthread_mutex_unlock(phi->check))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (-1);
	}
	if (pthread_mutex_unlock(phi->check))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	if (ft_standby(phi->params->time_to_eat))
		return (unlock_forks(phi) > 0 ? 1 : -1);
	++phi->has_eaten;
	if (unlock_forks(phi))
		return (1);
	if (phi->params->must_eat != -1 &&
	phi->has_eaten >= phi->params->must_eat)
		return (-1);
	return (0);
}
