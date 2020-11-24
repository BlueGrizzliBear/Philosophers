/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/24 16:51:05 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int		ft_is_dead(t_phi *phi)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	if (ft_get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
	{
		phi->status = 0;
		ft_display(phi, " died\n");
		return (1);
	}
	return (0);
}

void	*ft_in_order(void *arg)
{
	t_philo_three *p;
	t_phi	*iter;
	int		order;

	p = (t_philo_three*)(arg);
	iter = p->phi;
	order = 0;
	while (p->params->game == 1 && iter && p->params->nb != 0)
	{
		if (order == iter->id_nb)
		{
			if (sem_post(iter->order))
			{
				ft_error(ERROR_UNLOCK_SEM);
				return ((void*)0);
			}
			if (sem_wait(iter->order))
			{
				ft_error(ERROR_UNLOCK_SEM);
				return ((void*)0);
			}
			order = (order + 1) % p->params->nb;
		}
		iter = iter->next;
	}
	return ((void*)0);
}

int		ft_wait(t_philo_three *p)
{
	int		incr;
	int		status;
	t_phi	*iter;

	if (pthread_create(p->thread, NULL, &ft_in_order, p))
		return (ft_error(ERROR_CREATE_THREAD));
	pthread_detach(*p->thread);

	iter = p->phi;
	incr = 0;
	while (incr++ < p->params->nb)
	{
		status = 0;
		if (waitpid(-1, &status, 0) < 0)
			return (ft_error(ERROR_CREATE_FORK));
		if (WEXITSTATUS(status) >= 0)
		{
			p->params->game = 1;
			while (p->params->nb-- > 0)
			{
				kill(iter->pid, SIGINT);
				iter = iter->next;
			}
			return (0);
		}
	}
	return (0);
}

int		ft_is_alive(void *arg)
{
	t_phi	*phi;
	int		ret;

	phi = (t_phi *)arg;
	while (phi->params->game == 1)
	{
		ret = 0;
		if ((ret = ft_eat_sleep_think(phi)) < 0)
		{
			if (ret == -2 || ret == -3)
				ft_unlock_forks(phi);
			if (phi->status == 0)
				exit(2);
			exit(0);
		}
		else if (ret > 0)
			exit(1);
	}
	exit(0);
}

int		ft_launch(t_philo_three *p)
{
	t_phi	*iter;
	int		counter;
	int		pid;

	iter = p->phi;
	counter = p->params->nb;
	gettimeofday(&p->params->start, NULL);
	while (counter-- > 0)
	{
		if (sem_wait(iter->order))
			return (ft_error(ERROR_LOCK_SEM));
		gettimeofday(&iter->last_meal, NULL);
		if (!(pid = fork()))
			ft_is_alive(iter);
		if ((iter->pid = pid) < 0)
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
	if (ft_wait(p))
		return (ft_error(ERROR_JOIN_THREAD));
	if (ft_free(p))
		return (1);
	return (0);
}
