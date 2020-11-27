/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 13:26:57 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	lock_forks(t_phi *phi)
{
	if (sem_wait(phi->order_start))
		exit(ft_error(ERROR_LOCK_SEM));
	if (sem_wait(phi->params->forks))
		exit(ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n"))
		exit(ft_error(ERROR_DISPLAY));
	if (sem_wait(phi->params->forks))
		exit(ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n"))
		exit(ft_error(ERROR_DISPLAY));
	if (sem_post(phi->order_end))
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
	usleep(1000 * phi->params->time_to_eat);
	++phi->has_eaten;
	unlock_forks(phi);
	if (phi->params->must_eat != -1 &&
	phi->has_eaten >= phi->params->must_eat)
	{
		phi->status = 0;
		pthread_join(phi->brain, NULL);
		if (sem_post(phi->params->has_eaten))
			exit(ft_error(ERROR_UNLOCK_SEM));
		if (sem_wait(phi->stop))
			exit(ft_error(ERROR_LOCK_SEM));
	}
}

void	ft_sleep(t_phi *phi)
{
	if (ft_display(phi, " is sleeping\n") > 0)
		exit(ft_error(ERROR_DISPLAY));
	usleep(1000 * phi->params->time_to_sleep);
}

void	ft_think(t_phi *phi)
{
	if (ft_display(phi, " is thinking\n") > 0)
		exit(ft_error(ERROR_DISPLAY));
}
