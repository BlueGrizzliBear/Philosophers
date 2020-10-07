/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 12:42:40 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_standby(t_phi *phi, int time)
{
	struct timeval now;
	struct timeval standby_start;

	if (gettimeofday(&standby_start, NULL))
		return (ft_error(ERROR_GTOD));
	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	while (ft_get_timestamp(standby_start, now) < time)
	{
		if (ft_is_dead(phi) == 1 || phi->params->game == 0)
			return (-1);
		usleep(10);
		if (gettimeofday(&now, NULL))
			return (ft_error(ERROR_GTOD));
	}
	return (0);
}

int		ft_lock_forks(t_phi *phi)
{
	int ret;

	ret = 0;
	// while (phi->left_fork->id == phi->right_fork->id ||
	// 	(phi->left_fork->status == 1 || phi->right_fork->status == 1))
	// {
	// 	if (ft_is_dead(phi) == 1 || phi->params->game == 0)
	// 		return (1);
	// 	usleep(10);
	// }
	// here
	// if (pthread_mutex_lock(phi->params->available))
	// 	return (ft_error(ERROR_LOCK_MUTEX));
	// here
	// while (phi->left_fork->id == phi->right_fork->id ||
	// pthread_mutex_lock(phi->params->available) != 0 ||
	// phi->left_fork->status == 1 || phi->right_fork->status == 1)
	// {
	// 	if (pthread_mutex_unlock(phi->params->available))
	// 		return (ft_error(ERROR_UNLOCK_MUTEX));
	// 	if (ft_is_dead(phi) == 1 || phi->params->game == 0)
	// 		return (1);
	// 	usleep(10);
	// }

	if (pthread_mutex_lock(phi->left_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->left_fork->status = 1;
	if (pthread_mutex_lock(phi->right_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->right_fork->status = 1;
	// here
	// if (pthread_mutex_unlock(phi->params->available))
	// 	return (ft_error(ERROR_UNLOCK_MUTEX));
	// here
	if ((ret = ft_display(phi, "has taken a fork\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	if ((ret = ft_display(phi, "has taken a fork\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));

	// if (pthread_mutex_lock(phi->left_fork->mutex) ||
	// pthread_mutex_lock(phi->right_fork->mutex))
	// 	return (ft_error(ERROR_LOCK_MUTEX));
	// phi->left_fork->status = 1;
	// phi->right_fork->status = 1;
	// ret = 0;
	// if ((ret = ft_display(phi, "has taken a fork\n")) ||
	// (ret = ft_display(phi, "has taken a fork\n")))
	// 	return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	return (0);
}

int		ft_unlock_forks(t_phi *phi)
{
	phi->right_fork->status = 0;
	phi->left_fork->status = 0;
	if (pthread_mutex_unlock(phi->right_fork->mutex) ||
	pthread_mutex_unlock(phi->left_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	return (0);
}

int		ft_eat_sleep_think(t_phi *phi)
{
	int ret;

	ret = 0;
	if ((ret = ft_lock_forks(phi)) != 0)
		return (ret < 0 ? -2 : 1);
	if ((ret = ft_display(phi, "is eating\n")))
		return (ret < 0 ? -2 : ft_error(ERROR_DISPLAY));
	phi->has_eaten++;
	if (phi->params->nb_time_phi_must_eat != -1 &&
	phi->has_eaten >= phi->params->nb_time_phi_must_eat)
		return (-3);
	if (gettimeofday(&phi->last_meal, NULL))
		return (ft_error(ERROR_GTOD));
	if ((ret = ft_standby(phi, phi->params->time_to_eat)) != 0)
		return (ret < 0 ? -2 : ft_error(ERROR_STANDBY));
	if (ft_unlock_forks(phi))
		return (1);
	if ((ret = ft_display(phi, "is sleeping\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	if ((ret = ft_standby(phi, phi->params->time_to_sleep)) != 0)
		return (ret < 0 ? -1 : ft_error(ERROR_STANDBY));
	if ((ret = ft_display(phi, "is thinking\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	return (0);
}

void	*ft_is_alive(void *arg)
{
	t_phi	*phi;
	int		ret;

	phi = (t_phi *)arg;
	while (phi->params->game == 1)
	{
		ret = 0;
		if ((ret = ft_eat_sleep_think(phi)) != 0)
		{
			if (ret == -2 || ret == -3)
				ft_unlock_forks(phi);
			return (NULL);
		}
	}
	return (NULL);
}
