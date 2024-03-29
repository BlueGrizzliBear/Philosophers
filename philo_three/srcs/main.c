/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 15:44:46 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

void	ft_wait(t_philo_three *p)
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
		kill(iter->pid, SIGKILL);
		if (sem_post(iter->stop) || sem_post(iter->order_start) ||
		sem_post(iter->order_end))
			exit(ft_error(ERROR_UNLOCK_SEM));
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

int		ft_kill_prematured(int limit, t_phi *phi, t_philo_three *p)
{
	t_phi	*iter;
	int		i;

	p->params->game = 0;
	i = limit;
	iter = phi;
	while (i-- > 0)
	{
		kill(iter->pid, SIGKILL);
		if (sem_post(iter->stop) || sem_post(iter->order_start) ||
		sem_post(iter->order_end))
			return (ft_error(ERROR_UNLOCK_SEM));
		iter = iter->next;
	}
	if (sem_post(p->params->has_eaten))
		exit(ft_error(ERROR_UNLOCK_SEM));
	if (sem_post(p->params->game_over))
		exit(ft_error(ERROR_UNLOCK_SEM));
	pthread_join(p->ordering, NULL);
	if (p->params->must_eat != -1)
		pthread_join(p->has_eaten, NULL);
	return (ft_error(ERROR_CREATE_FORK));
}

int		ft_launch(t_philo_three *p)
{
	t_phi	*iter;
	int		counter;

	if (pthread_create(&p->ordering, NULL, &th_in_order, p))
		return (ft_error(ERROR_CREATE_THREAD));
	if (p->params->must_eat != -1 &&
	pthread_create(&p->has_eaten, NULL, &th_has_eaten, p))
	{
		pthread_detach(p->ordering);
		return (ft_error(ERROR_CREATE_THREAD));
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
			return (ft_kill_prematured(iter->id_nb, p->phi, p));
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
	{
		ft_free(p);
		return (ft_error(ERROR_LAUNCH_PHI));
	}
	ft_wait(p);
	ft_free(p);
	return (0);
}
