/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/20 15:45:50 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int		ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	int		nb;

	nb = params->nb;
	while (nb > 0)
	{
		iter = phi->next;
		free(phi);
		phi = iter;
		nb--;
	}
	phi = NULL;
	return (0);
}

int		ft_free_semaphore(t_params *params)
{
	sem_unlock(params->game_status);
	if (sem_close(params->game_status))
		return (ft_error(ERROR_CLOSING));
	if (sem_unlink("/game_status"))
		return (ft_error(ERROR_UNLINK));
	if (sem_close(params->display))
		return (ft_error(ERROR_CLOSING));
	if (sem_unlink("/display"))
		return (ft_error(ERROR_UNLINK));
	if (sem_close(params->forks))
		return (ft_error(ERROR_CLOSING));
	if (sem_unlink("/forks"))
		return (ft_error(ERROR_UNLINK));
	params->forks = NULL;
	return (0);
}

int		ft_free(t_philo_three *p)
{
	ft_free_philosophers(p->phi, p->params);
	if (ft_free_semaphore(p->params))
		return (1);
	free(p->params);
	p->params = NULL;
	free(p);
	p = NULL;
	return (0);
}
