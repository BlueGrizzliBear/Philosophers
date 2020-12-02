/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 15:27:02 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	*th_in_order(void *arg)
{
	t_philo_three	*p;
	t_phi			*iter;

	p = (t_philo_three*)(arg);
	iter = p->phi;
	while (p->params->game == 1)
	{
		if (sem_post(iter->order_start) && ft_error(ERROR_UNLOCK_SEM))
			return ((void*)0);
		if (sem_wait(iter->order_end) && ft_error(ERROR_LOCK_SEM))
			return ((void*)0);
		iter = iter->next;
	}
	return ((void*)0);
}

void	*th_has_eaten(void *arg)
{
	t_philo_three	*p;
	int				total;

	p = (t_philo_three*)arg;
	total = 0;
	while (total++ < p->params->nb && p->params->game == 1)
	{
		if (sem_wait(p->params->has_eaten) && ft_error(ERROR_LOCK_SEM))
			return ((void*)0);
	}
	if (sem_post(p->params->has_eaten) && ft_error(ERROR_UNLOCK_SEM))
		return ((void*)0);
	if (sem_post(p->params->game_over) && ft_error(ERROR_UNLOCK_SEM))
		return ((void*)0);
	return ((void*)0);
}
