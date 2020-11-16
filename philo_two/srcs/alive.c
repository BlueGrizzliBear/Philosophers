/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/16 15:33:43 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

int		ft_standby(t_phi *phi, int time)
{
	struct timeval now;
	struct timeval standby_start;

	gettimeofday(&standby_start, NULL);
	gettimeofday(&now, NULL);
	if (sem_wait(phi->params->game_status))
		return (ft_error(ERROR_LOCK_SEM));
	if (!phi->params->game)
	{
		if (sem_post(phi->params->game_status))
			return (ft_error(ERROR_UNLOCK_SEM));
		return (-1);
	}
	if (sem_post(phi->params->game_status))
		return (ft_error(ERROR_UNLOCK_SEM));
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

	ret = 0;
	while (phi->params->forks_nb < 2)
	{
		if (!phi->params->game || ft_is_dead(phi))
			return (-1);
	}
	phi->params->forks_nb -= 2;
	if (sem_wait(phi->params->forks) || sem_wait(phi->params->forks))
		return (ft_error(ERROR_LOCK_SEM));
	if ((ret = ft_display(phi, " has taken a fork\n")) ||
	(ret = ft_display(phi, " has taken a fork\n")))
		return (ret < 0 ? -1 : ft_error(ERROR_DISPLAY));
	return (0);
}

int		ft_unlock_forks(t_phi *phi)
{
	phi->params->forks_nb += 2;
	if (sem_post(phi->params->forks) || sem_post(phi->params->forks))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}

int		ft_eat_sleep_think(t_phi *phi)
{
	int ret;

	ret = 0;
	if ((ret = ft_lock_forks(phi)) != 0)
		return (ret < 0 ? -2 : 1);
	if ((ret = ft_display(phi, " is eating\n")))
		return (ret < 0 ? -2 : ft_error(ERROR_DISPLAY));
	phi->has_eaten++;
	if (phi->params->nb_time_phi_must_eat != -1 &&
	phi->has_eaten >= phi->params->nb_time_phi_must_eat)
		return (-3);
	gettimeofday(&phi->last_meal, NULL);
	if ((ret = ft_standby(phi, phi->params->time_to_eat)) != 0)
		return (ret < 0 ? -2 : ft_error(ERROR_STANDBY));
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
