/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/21 14:23:08 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_is_dead(t_phi *phi)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	if (ft_get_timestamp(phi->last_meal, now) >= phi->params->time_to_die)
	{
		if (pthread_mutex_lock(phi->params->game_status))
			return (ft_error(ERROR_LOCK_MUTEX));
		phi->status = 0;
		ft_display(phi, " died\n");
		phi->params->game = 0;
		if (pthread_mutex_unlock(phi->params->game_status))
			return (ft_error(ERROR_UNLOCK_MUTEX));
		return (1);
	}
	return (0);
}

int		ft_wait(t_philo_one *p)
{
	t_phi	*iter;
	int		counter;

	iter = p->phi;
	counter = p->params->nb;
	while (counter-- > 0)
	{
		if (pthread_join(*iter->thread, NULL))
			return (1);
		iter = iter->next;
	}
	return (0);
}

void	*ft_is_alive(void *arg)
{
	t_phi	*phi;
	int		ret;

	phi = (t_phi *)arg;
	while (phi->params->game == 1)
	{
		ret = 0;
		if ((ret = ft_eat_sleep_think(phi)) != 0)
		{
			if (ret == -2 || ret == -3)
				ft_unlock_forks(phi);
			return (NULL);
		}
	}
	return (NULL);
}

int		ft_launch(t_philo_one *p)
{
	t_phi			*iter;
	int				counter;

	iter = p->phi;
	counter = p->params->nb;
	gettimeofday(&p->params->start, NULL);
	while (counter-- > 0)
	{
		gettimeofday(&iter->last_meal, NULL);
		if (pthread_create(iter->thread, NULL, &ft_is_alive, iter))
			return (ft_error(ERROR_CREATE_THREAD));
		iter = iter->next;
		usleep(100);
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
		return (ft_error(ERROR_LAUNCH_PHI));
	if (ft_wait(p))
		return (ft_error(ERROR_JOIN_THREAD));
	if (ft_free(p))
		return (1);
	return (0);
}
