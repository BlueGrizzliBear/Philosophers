/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 14:35:18 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_is_dead(t_phi *phi)
{
	struct timeval now;

	if (gettimeofday(&now, NULL))
		return (ft_error(ERROR_GTOD));
	if (ft_get_timestamp(phi->last_meal, now) > phi->params->time_to_die)
	{
		phi->status = 0;
		ft_display(phi, "died\n");
		phi->params->game = 0;
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
	while (counter > 0)
	{
		if (pthread_join(*iter->thread, NULL))
			return (1);
		iter = iter->next;
		counter--;
	}
	return (0);
}

int		ft_launch(t_philo_one *p)
{
	t_phi			*iter;
	int				counter;

	iter = p->phi;
	counter = p->params->nb;
	if (gettimeofday(&p->params->start, NULL))
		return (ft_error(ERROR_GTOD));
	while (counter > 0)
	{
		if (gettimeofday(&iter->last_meal, NULL))
			return (ft_error(ERROR_GTOD));
		if (pthread_create(iter->thread, NULL, &ft_is_alive, iter))
			return (ft_error(ERROR_CREATE_THREAD));
		iter = iter->next;
		counter--;
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
