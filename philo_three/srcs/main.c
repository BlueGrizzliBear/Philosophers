/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 10:59:08 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	*th_in_order(void *arg)
{
	t_philo_three 	*p;
	t_phi			*iter;
	int				order;

	p = (t_philo_three*)(arg);
	iter = p->phi;
	order = 0;
	while (p->params->game == 1)
	{
		if (order == iter->id_nb)
		{
			if (sem_post(iter->order) && ft_error(ERROR_UNLOCK_SEM))
				return ((void*)0);
			if (sem_wait(iter->order) && ft_error(ERROR_LOCK_SEM))
				return ((void*)0);
			order = (order + 1) % p->params->nb;
		}
		iter = iter->next;
	}
	dprintf(2, "exiting th_in_order\n");
	return ((void*)0);
}

void	*th_has_eaten(void *arg)
{
	t_philo_three 	*p;
	int				total;

	p = (t_philo_three*)arg;
	total = 0;
	while (total++ < p->params->nb && p->params->game == 1)
	{
		if (sem_wait(p->params->has_eaten) && ft_error(ERROR_LOCK_SEM))
			return ((void*)0);
	}
	// p->params->game = 0;
	if (sem_post(p->params->has_eaten) && ft_error(ERROR_UNLOCK_SEM))
		return ((void*)0);
	if (sem_post(p->params->game_over) && ft_error(ERROR_UNLOCK_SEM))
		return ((void*)0);
	dprintf(2, "exiting th_has_eaten\n");
	return ((void*)0);
}

void	ft_wait(t_philo_three *p)
{
	t_phi	*iter;
	int		i;

	if (sem_wait(p->params->game_over))
		exit(ft_error(ERROR_LOCK_SEM));
	p->params->game = 0;
	iter = p->phi;
	usleep(1500);
	i = p->params->nb;
	while (i-- > 0)
	{
		kill(iter->pid, SIGKILL);
		if (sem_post(iter->stop))
			exit(ft_error(ERROR_UNLOCK_SEM));
		iter = iter->next;
	}
	if (sem_post(p->params->game_over))
		exit(ft_error(ERROR_UNLOCK_SEM));
}

int		ft_launch(t_philo_three *p)
{
	pthread_t	ordering;
	pthread_t	has_eaten;
	t_phi		*iter;
	int			counter;
	
	// Thread to insure the eating order
	if (pthread_create(&ordering, NULL, &th_in_order, p))
		return (ft_error(ERROR_CREATE_THREAD));
	pthread_detach(ordering);

	if (p->params->must_eat != -1)
	{
		if (pthread_create(&has_eaten, NULL, &th_has_eaten, p))
			return (ft_error(ERROR_CREATE_THREAD));
		pthread_detach(has_eaten);
	}

	iter = p->phi;
	counter = 0;
	gettimeofday(&p->params->start, NULL);
	while (counter++ < p->params->nb)
	{
		gettimeofday(&iter->last_meal, NULL);
		if (!(iter->pid = fork()))
			ft_is_alive(iter);
		if (iter->pid < 0)
			return (ft_error(ERROR_CREATE_FORK));
		iter = iter->next;
	}
	return (0);
}

int		main(int argc, char **argv)
{
	t_philo_three	*p;
	t_params		*params;

	if (!(argc >= 5 && argc <= 6))
		return (ft_error(ERROR_NB_ARG));
	if ((params = ft_parse(argv)) == NULL)
		return (ft_error(ERROR_ARG_VAL));
	if ((p = ft_init(params)) == NULL)
		return (ft_error(ERROR_INIT_STRUCT));
	if (ft_launch(p))
		return (ft_error(ERROR_LAUNCH_PHI));
	ft_wait(p);
	if (ft_free(p))
		return (1);
	return (0);
}
