/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 10:55:46 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int				ft_init_params(t_params *params, int val, int index)
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

pthread_mutex_t	*ft_create_mutex(void)
{
	pthread_mutex_t *mutex;

	if (!(mutex = malloc(sizeof(pthread_mutex_t))))
		return (NULL);
	if ((pthread_mutex_init(mutex, NULL)))
	{
		free(mutex);
		mutex = NULL;
		ft_error(ERROR_ALLOC_INIT_MUTEX);
		return (NULL);
	}
	return (mutex);
}

// t_params		*ft_create_params_mutexes(t_params *params)
// {
// 	if (!(params->display = ft_create_mutex()))
// 	{
// 		ft_free_params(params);
// 		return (NULL);
// 	}
// 	return (params);
// }

t_params		*ft_parse(char **argv)
{
	t_params	*params;
	int			i;
	int			val;

	params = NULL;
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
	if (!(params->display = ft_create_mutex()))
	{
		ft_free_params(params);
		return (NULL);
	}
	// params->all_has_eaten = 0;
	// if (!ft_create_params_mutexes(params))
	// {
	// 	return (NULL);
	// }
	return (params);
}
