/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 12:41:53 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

int		ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	int		nb;

	nb = params->nb;
	while (nb > 0)
	{
		free(phi->thread);
		phi->thread = NULL;
		iter = phi->next;
		free(phi);
		phi = iter;
		nb--;
	}
	phi = NULL;
	return (0);
}

int		ft_clean_semaphore(t_params *params)
{
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
	if (sem_close(params->order))
		return (ft_error(ERROR_CLOSING));
	if (sem_unlink("/order"))
		return (ft_error(ERROR_UNLINK));
	params->forks = NULL;
	return (0);
}

int		ft_free(t_philo_two *p)
{
	ft_free_philosophers(p->phi, p->params);
	if (ft_clean_semaphore(p->params))
		return (1);
	free(p->params);
	p->params = NULL;
	free(p);
	p = NULL;
	return (0);
}