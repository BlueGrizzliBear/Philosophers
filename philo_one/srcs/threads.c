/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 11:42:50 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		get_order(int order, int nb)
{
	int	delta;
	int	new_order;

	delta = nb % 2;
	new_order = order + 2;
	if (delta == 0 && new_order > nb)
		new_order = (new_order - 1) % nb;
	else if (delta == 0 && new_order == nb)
		new_order = (new_order + 1) % nb;
	else
		new_order = new_order % nb;
	return (new_order);
}

int		unlock_order_mutexes(t_philo_one *p)
{
	t_phi	*iter;
	int		nb;

	iter = p->phi;
	nb = 0;
	while (nb++ < p->params->nb)
	{
		if (pthread_mutex_unlock(iter->order) && ft_error(ERROR_UNLOCK_MUTEX))
			return (1);
		iter = iter->next;
	}
	return (0);
}

void	*th_in_order(void *arg)
{
	t_philo_one	*p;
	t_phi		*i;
	int			order;

	p = (t_philo_one*)(arg);
	i = p->phi;
	order = 0;
	while (p->params->game == 1 && i->ordo == 1)
	{
		if (order == i->id_nb)
		{
			if (pthread_mutex_unlock(i->order) && ft_error(ERROR_UNLOCK_MUTEX))
				return ((void*)0);
			while (i->ordo == 1)
				ft_standby(1);
			i->ordo = 1;
			if (pthread_mutex_lock(i->order) && ft_error(ERROR_LOCK_MUTEX))
				return ((void*)0);
			order = get_order(order, p->params->nb);
		}
		i = i->next;
	}
	unlock_order_mutexes(p);
	return ((void*)0);
}

void	*th_has_eaten(void *arg)
{
	t_philo_one	*p;
	int			total;
	t_phi		*iter;

	p = (t_philo_one*)arg;
	iter = p->phi;
	total = 0;
	while (total < p->params->nb && p->params->game == 1)
	{
		if (iter->has_eaten == p->params->must_eat)
		{
			iter = iter->next;
			total++;
		}
	}
	p->params->game = 0;
	return ((void*)0);
}
