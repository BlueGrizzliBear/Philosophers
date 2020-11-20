/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/20 14:55:44 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

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
		params->nb_time_phi_must_eat = val;
	return (0);
}

char	*ft_fill_const_char(char *c, char *str)
{
	int i;

	i = -1;
	memset((void*)c, '\0', 32);
	dprintf(2, "%lu\n", sizeof((const char*)c));
	while (str[++i])
		c[i] = str[i];		
	return (c);
}

t_params	*ft_create_sem(t_params *params)
{
	char str[32];

	ft_fill_const_char(str, "/game_status");

	sem_unlink((const char*)str);
	params->game_status = sem_open(str, O_CREAT, S_IRWXU, 1);
	if (params->game_status == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM) ? NULL : NULL);
	sem_unlink("/display");
	params->display = sem_open("/display", O_CREAT, S_IRWXU, 1);
	if (params->display == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM) ? NULL : NULL);
	sem_unlink("/forks");
	params->forks = sem_open("/forks", O_CREAT, S_IRWXU, params->nb);
	if (params->forks == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM) ? NULL : NULL);
	return (params);
}

t_params	*ft_parse(char **argv)
{
	t_params	*params;
	int			i;
	int			val;

	if (!(params = malloc(sizeof(t_params))))
		return (ft_error(ERROR_STRUCT_CREAT) == 3 ? NULL : NULL);
	params->nb_time_phi_must_eat = -1;
	i = 0;
	while (argv[++i])
	{
		if ((val = ft_atoi(argv[i])) < 1)
			return (NULL);
		ft_init_params(params, val, i);
	}
	params->game = 1;
	if (!(params = ft_create_sem(params)))
		return (NULL);
	params->forks_nb = params->nb;
	return (params);
}
