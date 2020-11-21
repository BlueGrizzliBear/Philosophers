/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/21 10:00:32 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_is_over(t_phi *phi)
{
	if (pthread_mutex_lock(phi->params->game_status))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (!phi->params->game)
	{
		if (pthread_mutex_unlock(phi->params->game_status))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (-1);
	}
	if (pthread_mutex_unlock(phi->params->game_status))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		ft_standby(t_phi *phi, int time)
{
	struct timeval now;
	struct timeval standby_start;

	gettimeofday(&standby_start, NULL);
	gettimeofday(&now, NULL);
	if (ft_is_over(phi))
		return (-1);
	while (ft_get_timestamp(standby_start, now) < time)
	{
		if (!phi->params->game || ft_is_dead(phi))
			return (-1);
		gettimeofday(&now, NULL);
	}
	return (0);
}

int		ft_lock_forks(t_phi *phi)
{
	int ret;

	while (phi->right_fork->status == 1 ||
	phi->left_fork->mutex == phi->right_fork->mutex)
	{
		if (!phi->params->game || ft_is_dead(phi))
			return (-1);
	}
	if (pthread_mutex_lock(phi->left_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	if (pthread_mutex_lock(phi->right_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->right_fork->status = 1;
	phi->left_fork->status = 1;
	ret = 0;
	if ((ret = ft_display(phi, " has taken a fork\n")) ||
	(ret = ft_display(phi, " has taken a fork\n")))
		return (ret < 0 ? -2 : ft_error(ERROR_DISPLAY));
	return (0);
}

int		ft_unlock_forks(t_phi *phi)
{
	phi->right_fork->status = 0;
	phi->left_fork->status = 0;
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	if (pthread_mutex_unlock(phi->right_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		ft_eat_sleep_think(t_phi *phi)
{
	int ret;

	ret = 0;
	if ((ret = ft_lock_forks(phi)) != 0)
		return (ret < 0 ? ret : 1);
	if ((ret = ft_display(phi, " is eating\n")))
		return (ret < 0 ? -2 : ft_error(ERROR_DISPLAY));
	gettimeofday(&phi->last_meal, NULL);
	if ((ret = ft_standby(phi, phi->params->time_to_eat)) != 0)
		return (ret < 0 ? -2 : ft_error(ERROR_STANDBY));
	if (phi->params->nb_time_phi_must_eat != -1 &&
	++phi->has_eaten >= phi->params->nb_time_phi_must_eat)
		return (-3);
	if (ft_unlock_forks(phi))
		return (1);
	if ((ret = ft_display(phi, " is sleeping\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	if ((ret = ft_standby(phi, phi->params->time_to_sleep)) != 0)
		return (ret < 0 ? -1 : ft_error(ERROR_STANDBY));
	if ((ret = ft_display(phi, " is thinking\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	return (0);
}
