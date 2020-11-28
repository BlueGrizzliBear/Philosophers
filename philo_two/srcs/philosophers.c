/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/28 11:10:12 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

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
	if (sem_wait(phi->order_start))
		return (ft_error(ERROR_LOCK_SEM));
	if (sem_wait(phi->params->forks))
		return (ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n", 0))
		return (-1);
	if (sem_wait(phi->params->forks))
		return (ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " has taken a fork\n", 0))
		return (-1);
	if (sem_post(phi->order_end))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}

int		unlock_forks(t_phi *phi)
{
	if (sem_post(phi->params->forks) || sem_post(phi->params->forks))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}

int		ft_eat(t_phi *phi)
{
	if (lock_forks(phi))
		return (-1);
	if (sem_wait(phi->check))
		return (ft_error(ERROR_LOCK_SEM));
	if (ft_display(phi, " is eating\n", 0))
		return (unlock_forks(phi) > 0 ? 1 : -1);
	gettimeofday(&phi->last_meal, NULL);
	if (sem_post(phi->check))
		return (ft_error(ERROR_UNLOCK_SEM));
	if (ft_standby(phi->params->time_to_eat))
		return (unlock_forks(phi) > 0 ? 1 : -1);
	++phi->has_eaten;
	if (unlock_forks(phi))
		return (1);
	if (phi->params->must_eat != -1 &&
	phi->has_eaten >= phi->params->must_eat)
	{
		phi->status = 0;
		pthread_join(phi->brain, NULL);
		if (sem_post(phi->params->has_eaten))
			return (ft_error(ERROR_UNLOCK_SEM));
		if (sem_wait(phi->stop))
			return (ft_error(ERROR_LOCK_SEM));
	}
	return (0);
}
