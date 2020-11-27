/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 11:21:16 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int		ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	t_phi	*iter_next;
	int		nb;

	iter = phi;
	nb = params->nb;
	while (nb-- > 0)
	{
		if (sem_close(iter->order))
			return (ft_error(ERROR_CLOSING));
		// if (sem_unlink("/order"))
		// {
		// 	dprintf(2, "1errno|%s|\n", strerror(errno));
		// 	return (ft_error(ERROR_UNLINK));
		// }
		iter->order = NULL;
		
		if (sem_close(iter->check))
			return (ft_error(ERROR_CLOSING));
		// if (sem_unlink("/check"))
		// {
		// 	dprintf(2, "2errno|%s|\n", strerror(errno));
		// 	return (ft_error(ERROR_UNLINK));
		// }
		iter->check = NULL;
		
		if (sem_close(iter->stop))
			return (ft_error(ERROR_CLOSING));
		// if (sem_unlink("/stop"))
		// {
		// 	dprintf(2, "3errno|%s|\n", strerror(errno));
		// 	return (ft_error(ERROR_UNLINK));
		// }
		iter->stop = NULL;
		iter_next = iter->next;
		free(iter);
		iter = iter_next;
	}
	iter = NULL;
	return (0);
}

int		ft_free_semaphore(t_params *params)
{
	if (sem_close(params->has_eaten))
		return (ft_error(ERROR_CLOSING));
	// if (sem_unlink("/has_eaten"))
	// 	return (ft_error(ERROR_UNLINK));
	params->has_eaten = NULL;
	if (sem_close(params->game_over))
		return (ft_error(ERROR_CLOSING));
	// if (sem_unlink("/game_over"))
	// 	return (ft_error(ERROR_UNLINK));
	params->game_over = NULL;
	if (sem_close(params->display))
		return (ft_error(ERROR_CLOSING));
	// if (sem_unlink("/display"))
	// 	return (ft_error(ERROR_UNLINK));
	params->display = NULL;
	if (sem_close(params->forks))
		return (ft_error(ERROR_CLOSING));
	// if (sem_unlink("/forks"))
	// 	return (ft_error(ERROR_UNLINK));
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
