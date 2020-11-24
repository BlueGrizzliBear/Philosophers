/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/24 12:10:49 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int		ft_is_dead(t_phi *phi)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	if (ft_get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
	{
		// if (sem_wait(phi->params->game_status))
		// 	return (ft_error(ERROR_LOCK_SEM));
		phi->status = 0;
		ft_display(phi, " died\n");
		phi->params->game = 0;
		// if (sem_post(phi->params->game_status))
		// 	return (ft_error(ERROR_UNLOCK_SEM));
		return (1);
	}
	return (0);
}

int		ft_wait(t_philo_three *p)
{
	t_phi	*iter;
	int		status;
	int		incr;

	iter = p->phi;
	incr = 0;
	while (incr++ < p->params->nb)
	{
		status = 0;
		if (waitpid(-1, &status, 0) < 0)
			return (ft_error(ERROR_CREATE_FORK));
		if (WEXITSTATUS(status) > 0)
		{
			status = p->params->nb;
			while (status > 0)
			{
				kill(iter->pid, SIGINT);
				iter = iter->next;
				status--;
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
