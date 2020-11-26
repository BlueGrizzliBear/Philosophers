/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/26 16:52:27 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int			ft_init_params(t_params *params, int val, int index)
{
	if (index == 1)
		params->nb = val;
	else if (index == 2)
		params->time_to_die = val;
	else if (index == 3)
		params->time_to_eat = val;
	else if (index == 4)
		params->time_to_sleep = val;
	else if (index == 5)
		params->must_eat = val;
	return (0);
}

int			ft_init_semaphores(t_params *params)
{
	params->game = 1;
	if (params->must_eat != -1)
	{
		sem_unlink("/has_eaten");
		params->has_eaten = sem_open("/has_eaten", O_CREAT, S_IRWXU, 0);
		if (params->has_eaten == SEM_FAILED)
			return (ft_error(ERROR_OPEN_SEM));
	}
	sem_unlink("/game_over");
	params->game_over = sem_open("/game_over", O_CREAT, S_IRWXU, 0);
	if (params->game_over == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	sem_unlink("/display");
	params->display = sem_open("/display", O_CREAT, S_IRWXU, 1);
	if (params->display == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	sem_unlink("/forks");
	params->forks = sem_open("/forks", O_CREAT, S_IRWXU, params->nb);
	if (params->forks == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	return (0);
}

t_params	*ft_parse(char **argv)
{
	t_params	*params;
	int			i;
	int			val;

	if (!(params = malloc(sizeof(t_params))))
		return (ft_error(ERROR_STRUCT_CREAT) == 3 ? NULL : NULL);
	params->must_eat = -1;
	i = 0;
	while (argv[++i])
	{
		if ((val = ft_atoi(argv[i])) < 1)
			return (NULL);
		ft_init_params(params, val, i);
	}
	if (ft_init_semaphores(params))
		return (NULL);
	return (params);
}
