/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/29 17:34:26 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

void	*th_in_order(void *arg)
{
	t_philo_two	*p;
	t_phi		*iter;

	p = (t_philo_two*)(arg);
	iter = p->phi;
	while (p->params->game == 1)
	{
		// dprintf(2, "will order phi|%d| to eat\n", iter->id_nb);
		if (sem_post(iter->order_start) && ft_error(ERROR_UNLOCK_SEM))
			return ((void*)0);
		// dprintf(2, "ordered phi|%d| to eat\n", iter->id_nb);
		if (sem_wait(iter->order_end) && ft_error(ERROR_LOCK_SEM))
			return ((void*)0);
		// dprintf(2, "phi|%d| took forks\n", iter->id_nb);
		iter = iter->next;
	}
	return ((void*)0);
}

void	*th_has_eaten(void *arg)
{
	t_philo_two	*p;
	int			total;

	p = (t_philo_two*)arg;
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

void	ft_wait(t_philo_two *p)
{
	t_phi	*iter;
	int		i;

	if (sem_wait(p->params->game_over))
		exit(ft_error(ERROR_LOCK_SEM));
	p->params->game = 0;
	iter = p->phi;
	i = p->params->nb;
	while (i-- > 0)
	{
		if (sem_post(iter->params->display) || sem_post(iter->params->forks) ||
		sem_post(iter->params->forks) || sem_post(iter->order_start) ||
		sem_post(iter->order_end))
			exit(ft_error(ERROR_UNLOCK_SEM));
		pthread_join(iter->entity, NULL);
		iter = iter->next;
	}
	if (sem_post(p->params->has_eaten))
		exit(ft_error(ERROR_UNLOCK_SEM));
	if (sem_post(p->params->game_over))
		exit(ft_error(ERROR_UNLOCK_SEM));
	pthread_join(p->ordering, NULL);
	if (p->params->must_eat != -1)
		pthread_join(p->has_eaten, NULL);
}

int		ft_launch(t_philo_two *p)
{
	t_phi	*iter;
	int		counter;

	if (pthread_create(&p->ordering, NULL, &th_in_order, p))
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
	t_philo_two	*p;
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
