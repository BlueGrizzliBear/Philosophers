/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/01 17:23:57 by cbussier         ###   ########lyon.fr   */
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

int		lock_forks(t_phi *phi)
{
	while (phi->right_fork->status == 1 ||
	phi->left_fork->mutex == phi->right_fork->mutex)
	{
		usleep(1);
	}
	if (pthread_mutex_lock(phi->left_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->left_fork->status = 1;
	if (ft_display(phi, " has taken a fork\n", 0))
		return (-1);
	if (pthread_mutex_lock(phi->right_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->right_fork->status = 1;
	if (ft_display(phi, " has taken a fork\n", 0))
		return (-1);
	return (0);
}

int		unlock_forks(t_phi *phi)
{
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	phi->left_fork->status = 0;
	if (pthread_mutex_unlock(phi->right_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	phi->right_fork->status = 0;
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
		return (unlock_forks(phi) > 0 ? 1 : -1);
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
