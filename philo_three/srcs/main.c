/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/26 14:21:10 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	*ft_in_order(void *arg)
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
	return ((void*)0);
}

void	ft_wait(t_philo_three *p)
{
	int		status;
	t_phi	*iter;

	status = 0;
	if (sem_wait(p->params->game_over))
		exit(ft_error(ERROR_LOCK_SEM));
	p->params->game = 0;
	// usleep(1000000);
	iter = p->phi;
	while (p->params->nb-- > 0)
	{
		kill(iter->pid, SIGINT);
		iter = iter->next;
	}
	if (sem_post(p->params->game_over))
		exit(ft_error(ERROR_UNLOCK_SEM));
}

int		ft_launch(t_philo_three *p)
{
	pthread_t	ordering;
	t_phi		*iter;
	int			counter;
	
	// Thread to insure the eating order
	if (pthread_create(&ordering, NULL, &ft_in_order, p))
		return (ft_error(ERROR_CREATE_THREAD));
	pthread_detach(ordering);
	
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
