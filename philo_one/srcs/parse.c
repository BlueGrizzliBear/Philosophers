/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/02 15:56:44 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

t_params	*ft_parse(char **argv)
{
	t_params *params;
	int i;
	int	val;

	i = 1;
	if (!(params = malloc(sizeof(t_params))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	params->nb_time_phi_must_eat = -1;
	while (argv[i])
	{
		if ((val = ft_atoi(argv[i])) < 1)
			return (NULL);
		if (i == 1)
			params->nb = val;
		else if (i == 2)
			params->time_to_die = val;
		else if (i == 3)
			params->time_to_eat = val;
		else if (i == 4)
			params->time_to_sleep = val;
		else if (i == 5)
			params->nb_time_phi_must_eat = val;
		i++;
	}
	return (params);
}