/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/26 16:32:46 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

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

t_params	*ft_create_mutex(t_params *params)
{
	if (!(params->game_status = malloc(sizeof(pthread_mutex_t))) ||
	(pthread_mutex_init(params->game_status, NULL)))
	{
		ft_error(ERROR_ALLOC_INIT_MUTEX);
		return (NULL);
	}
	if (!(params->display = malloc(sizeof(pthread_mutex_t))) ||
	(pthread_mutex_init(params->display, NULL)))
	{
		ft_error(ERROR_ALLOC_INIT_MUTEX);
		return (NULL);
	}
	return (params);
}

t_params	*ft_parse(char **argv)
{
	t_params	*params;
	int			i;
	int			val;

	if (!(params = malloc(sizeof(t_params))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	params->must_eat = -1;
	i = 0;
	while (argv[++i])
	{
		if ((val = ft_atoi(argv[i])) < 1)
			return (NULL);
		ft_init_params(params, val, i);
	}
	params->game = 1;
	if (!(params = ft_create_mutex(params)))
		return (NULL);
	return (params);
}