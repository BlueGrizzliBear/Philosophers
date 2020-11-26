/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/26 17:20:04 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	ft_standby(int time)
{
	usleep(1000 * time);
	// struct timeval now;
	// struct timeval standby_start;

	// gettimeofday(&standby_start, NULL);
	// gettimeofday(&now, NULL);
	// while (get_timestamp(standby_start, now) < time)
	// 	gettimeofday(&now, NULL);
}

void	lock_forks(t_phi *phi)
{
	// wait for order given
	if (sem_wait(phi->order))
		exit(ft_error(ERROR_LOCK_SEM));
	if (sem_wait(phi->params->forks))
		exit(ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n"))
		exit(ft_error(ERROR_DISPLAY));
	if (sem_wait(phi->params->forks))
		exit(ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n"))
		exit(ft_error(ERROR_DISPLAY));
	// give back order
	if (sem_post(phi->order))
		exit(ft_error(ERROR_UNLOCK_SEM));
}

void	unlock_forks(t_phi *phi)
{
	if (sem_post(phi->params->forks) || sem_post(phi->params->forks))
		exit(ft_error(ERROR_UNLOCK_SEM));
}

void	ft_eat(t_phi *phi)
{
	lock_forks(phi);
	if (sem_wait(phi->check))
		exit(ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " is eating\n") > 0)
		exit(ft_error(ERROR_DISPLAY));
	gettimeofday(&phi->last_meal, NULL);
	if (sem_post(phi->check))
		exit(ft_error(ERROR_UNLOCK_SEM));
	ft_standby(phi->params->time_to_eat);
	++phi->has_eaten;
	unlock_forks(phi);
	if (phi->params->must_eat != -1 &&
	phi->has_eaten >= phi->params->must_eat)
	{
		// dprintf(2, "phi|%d| quiting here\n", phi->id_nb);
		if (sem_post(phi->params->has_eaten))
			exit(ft_error(ERROR_UNLOCK_SEM));
		exit(0);
	}
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

int		ft_is_dead(t_phi *phi)
{
	static struct timeval now;

	gettimeofday(&now, NULL);
	if (get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
		return (1);
	return (0);
}

void	*ft_brain(void *arg)
{
	t_phi	*phi;
	int		end;

	phi = (t_phi*)(arg);
	end = 0;
	while (1)
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
		// if (phi->has_eaten == phi->params->must_eat)
		// {
		// 	dprintf(2, "END phi|%d| eaten|%d|\n", phi->id_nb, phi->has_eaten);			
		// 	return ((void*)0);
		// }
		if (sem_post(phi->check) && ft_error(ERROR_UNLOCK_SEM))
			return ((void*)0);
		usleep(1000);
	}
	return ((void*)0);
}

int		ft_is_alive(void *arg)
{
	pthread_t	brain;
	t_phi		*phi;

	phi = (t_phi *)arg;

	// Thread to check if philosopher is alive, equivalent to BRAIN
	if (pthread_create(&brain, NULL, &ft_brain, phi))
		exit(ft_error(ERROR_CREATE_THREAD));
	pthread_detach(brain);

	while (1)
	{
		ft_eat(phi);
		ft_sleep(phi);
		ft_think(phi);
	}
	exit(0);
}
