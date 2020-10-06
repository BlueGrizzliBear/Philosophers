/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/06 21:25:22 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

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
	while (phi->params->forks_nb < 2)
	{
		if (ft_is_dead(phi) == 1 || phi->params->game == 0)
			return (-1);
		usleep(10);
	}
	if (sem_wait(phi->params->forks) || sem_wait(phi->params->forks))
		return (ft_error(ERROR_LOCK_SEM));
	if ((ret = ft_display(phi, "has taken a fork\n")) ||
	(ret = ft_display(phi, "has taken a fork\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	phi->params->forks_nb -= 2;
	return (0);
}

int		ft_unlock_forks(t_phi *phi)
{
	if (sem_post(phi->params->forks) || sem_post(phi->params->forks))
		return (ft_error(ERROR_UNLOCK_SEM));
	phi->params->forks_nb += 2;
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

int		ft_is_alive(void *arg)
{
	t_phi	*phi;
	int		ret;

	phi = (t_phi *)arg;
	while (phi->params->game == 1)
	{
		ret = 0;
		if ((ret = ft_eat_sleep_think(phi)) < 0)
		{
			if (ret == -2 || ret == -3)
				ft_unlock_forks(phi);
			if (phi->status == 0)
				exit(2);
			exit(0);
		}
		else if (ret > 0)
			exit(1);
	}
	return (0);
}
