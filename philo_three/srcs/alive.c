/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 14:36:05 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

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
	while (phi->status == 1)
	{
		if (sem_wait(phi->check) && ft_error(ERROR_LOCK_SEM))
			return ((void*)0);
		if (phi->has_eaten != phi->params->must_eat && ft_is_dead(phi))
		{
			phi->status = 0;
			ft_display(phi, " died\n");
			if (sem_post(phi->check) && ft_error(ERROR_UNLOCK_SEM))
				return ((void*)0);
			if (sem_post(phi->params->game_over) && ft_error(ERROR_UNLOCK_SEM))
				return ((void*)0);
			return ((void*)0);
		}
		if (sem_post(phi->check) && ft_error(ERROR_UNLOCK_SEM))
			return ((void*)0);
		usleep(1000);
	}
	return ((void*)0);
}

void	ft_sleep(t_phi *phi)
{
	if (ft_display(phi, " is sleeping\n") > 0)
		exit(ft_error(ERROR_DISPLAY));
	ft_standby(phi->params->time_to_sleep);
}

void	ft_think(t_phi *phi)
{
	if (ft_display(phi, " is thinking\n") > 0)
		exit(ft_error(ERROR_DISPLAY));
}

int		ft_is_alive(void *arg)
{
	t_phi		*phi;

	phi = (t_phi *)arg;
	if (pthread_create(&phi->brain, NULL, &th_brain, phi))
		exit(ft_error(ERROR_CREATE_THREAD));
	while (1)
	{
		ft_eat(phi);
		ft_sleep(phi);
		ft_think(phi);
	}
	exit(0);
}
