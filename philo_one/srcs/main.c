/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/03 18:21:28 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_wait(t_philo_one *p)
{
	t_phi			*iter;
	int				counter;

	iter = p->phi;
	counter = p->params->nb;	
	while (counter > 0)
	{
		if (pthread_join(*iter->thread, NULL))
			return (1);
		dprintf(2, "joined phi %d\n", iter->id);
		// pthread_detach(*iter->thread);
		// dprintf(2, "detached phi %d\n", iter->id);
		iter = iter->next;
		counter--;
	}
	// while (counter > 0)
	// {
	// 	pthread_detach(*iter->thread);
	// 	iter = iter->next;
	// 	counter--;
	// }
	// usleep(20000000);
	return (0);
}

int		ft_launch(t_philo_one *p)
{
	t_phi			*iter;
	int				counter;

	iter = p->phi;
	counter = p->params->nb;
	while (counter > 0)
	{
		if (gettimeofday(&iter->start, NULL))
			return (ft_error(ERROR_GTOD));
		if (gettimeofday(&iter->last_meal, NULL))
			return (ft_error(ERROR_GTOD));
		// iter->alive = 1;
		if (pthread_create(iter->thread, NULL, &ft_is_alive, iter))
			return (ft_error(ERROR_CREATE_THREAD));
		iter = iter->next;
		counter--;
	}
	// while (iter->alive == 1 && 
	// (iter->params->nb_time_phi_must_eat == -1 || iter->has_eaten < iter->params->nb_time_phi_must_eat))
	// 	iter = iter->next;
	// if (iter->alive == 0)
	// 	dprintf(2, "SOMEONE DIED\n");
	// else
	// 	dprintf(2, "END OF SIMULATION = TIME EATEN\n");
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
	// dprintf(2, "Free-ing programm\n");
	ft_free(p);
	// dprintf(2, "Ending programm\n");
	return (0);
}
