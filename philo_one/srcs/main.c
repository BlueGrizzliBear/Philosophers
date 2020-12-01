/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/01 22:58:20 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

void	*th_in_order(void *arg)
{
	t_philo_one	*p;
	t_phi		*iter;
	int			order;
	int			delta;

	p = (t_philo_one*)(arg);
	iter = p->phi;
	order = 0;
	delta = p->params->nb % 2;
	while (p->params->game == 1)
	{
		if (order == iter->id_nb)
		{
			dprintf(2, "ordering phi|%d|\n", iter->id_nb);
			if (pthread_mutex_unlock(iter->order) && ft_error(ERROR_UNLOCK_MUTEX))
				return ((void*)0);
			while (iter->ordo == 1)
				ft_standby(1);
			iter->ordo = 1;
			// dprintf(2, "waiting for phi|%d|\n", iter->id_nb);
			if (pthread_mutex_lock(iter->order) && ft_error(ERROR_LOCK_MUTEX))
				return ((void*)0);
			// dprintf(2, "came back|%d|\n", iter->id_nb);
			order += 2;
			if (delta == 0 && order > p->params->nb)
				order = (order - 1) % p->params->nb;
			else if (delta == 0 && order == p->params->nb)
				order = (order + 1) % p->params->nb;
			else
				order = order % p->params->nb;
			// dprintf(2, "going next for |%d|\n", order);
		}
		iter = iter->next;
	}
	order = 0;
	iter = p->phi;
	while (order++ < p->params->nb)
	{
		if (pthread_mutex_unlock(iter->order) && ft_error(ERROR_UNLOCK_MUTEX))
			return ((void*)0);
		iter = iter->next;
	}
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
	p->params->all_has_eaten = total;
	if (pthread_mutex_unlock(p->params->game_over) &&
	ft_error(ERROR_UNLOCK_MUTEX))
		return ((void*)0);
	return ((void*)0);
}

void	ft_wait(t_philo_one *p)
{
	t_phi	*iter;
	int		i;

	if (pthread_mutex_lock(p->params->game_over))
		exit(ft_error(ERROR_LOCK_MUTEX));
	p->params->game = 0;
	iter = p->phi;
	i = 0;
	while (i++ < p->params->nb)
	{
		pthread_join(iter->entity, NULL);
		iter = iter->next;
	}
	if (pthread_mutex_unlock(p->params->game_over))
		exit(ft_error(ERROR_UNLOCK_MUTEX));
	pthread_join(p->in_order, NULL);
	if (p->params->must_eat != -1)
		pthread_join(p->has_eaten, NULL);
}

int		ft_launch(t_philo_one *p)
{
	t_phi	*iter;
	int		counter;

	if (pthread_create(&p->in_order, NULL, &th_in_order, p))
		return (ft_error(ERROR_CREATE_THREAD));
	if (p->params->must_eat != -1)
	{
		if (pthread_create(&p->has_eaten, NULL, &th_has_eaten, p))
			return (ft_error(ERROR_CREATE_THREAD));
	}
	iter = p->phi;
	counter = 0;
	gettimeofday(&p->params->start, NULL);
	while (counter++ < p->params->nb)
	{
		gettimeofday(&iter->last_meal, NULL);
		if (pthread_create(&iter->entity, NULL, &th_is_alive, iter))
			return (ft_error(ERROR_CREATE_THREAD));
		iter = iter->next;
	}
	return (0);
}

int		main(int argc, char **argv)
{
	t_philo_one	*p;
	t_params	*params;

	if (!(argc >= 5 && argc <= 6))
		return (ft_error(ERROR_NB_ARG));
	if ((params = ft_parse(argv)) == NULL)
		return (ft_error(ERROR_ARG_VAL));
	if ((p = ft_init(params)) == NULL)
		return (ft_error(ERROR_INIT_STRUCT));
	if (ft_launch(p))
	{
		ft_free(p);
		return (ft_error(ERROR_LAUNCH_PHI));
	}
	ft_wait(p);
	ft_free(p);
	return (0);
}
