/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/03 18:12:12 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_is_dead(t_phi *phi)
{
	struct timeval now;

	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	if (ft_get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
	{
		phi->alive = 0;
		ft_display(phi, "died\n");
		return (1);
	}
	return (0);
}

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
		// if (*phi->game == 0)
		// 	return (-2);
		if (ft_is_dead(phi) == 1 || *phi->game == 0)
			return (-1);
		usleep(10);
		if (gettimeofday(&now, NULL))
			return (ft_error(ERROR_GTOD));
	}
	return (0);
}

int		ft_lock_forks(t_phi *phi)
{
	while (phi->right_fork->status == 1 || phi->left_fork->status == 1)
	{
		if (ft_is_dead(phi) == 1)
			return (-1);
		usleep(10);
	}
	if (pthread_mutex_lock(phi->left_fork->mutex) ||
	pthread_mutex_lock(phi->right_fork->mutex))
		return (ft_error(ERROR_LOCK_MUTEX));
	phi->right_fork->status = 1;
	phi->left_fork->status = 1;
	return (0);
}

int		ft_unlock_forks(t_phi *phi)
{
	if (pthread_mutex_unlock(phi->left_fork->mutex) ||
	pthread_mutex_unlock(phi->right_fork->mutex))
		return (ft_error(ERROR_UNLOCK_MUTEX));
	phi->right_fork->status = 0;
	phi->left_fork->status = 0;
	return (0);
}

int		ft_eat_sleep_think(t_phi *phi)
{
	int ret;

	if (ft_lock_forks(phi))
		return (1);
	if (ft_display(phi, "has taken fork\n") || ft_display(phi, "is eating\n"))
		return (ft_error(ERROR_DISPLAY));
	phi->has_eaten++;
	if (phi-> params->nb_time_phi_must_eat != -1 &&
	phi->has_eaten >= phi->params->nb_time_phi_must_eat)
		return (-2);
	if (gettimeofday(&phi->last_meal, NULL))
		return (ft_error(ERROR_GTOD));
	ret = 0;
	if ((ret = ft_standby(phi, phi->params->time_to_eat)) != 0)
		return (ret < 0 ? -2 : ft_error(ERROR_STANDBY));
	if (ft_unlock_forks(phi))
		return (1);
	if (ft_display(phi, "is sleeping\n"))
		return (ft_error(ERROR_DISPLAY));
	ret = 0;
	if ((ret = ft_standby(phi, phi->params->time_to_sleep)) != 0)
		return (ret < 0 ? -1 : ft_error(ERROR_STANDBY));
	if (ft_display(phi, "is thinking\n"))
		return (ft_error(ERROR_DISPLAY));
	return (0);
}

void	*ft_is_alive(void *arg)
{
	t_phi	*phi;
	int		ret;

	phi = (t_phi *)arg;
	while (*phi->game == 1)
	{
		ret = 0;
		if ((ret = ft_eat_sleep_think(phi)) < 0)
		{
			*phi->game = 0;
			if (ret == -2)
				ft_unlock_forks(phi);
			// pthread_detach(*phi->thread);
			// dprintf(2, "detached phi %d\n", phi->id);
			return (NULL);
		}
	}
	return (NULL);
}
