/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 14:38:06 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

void	ft_free_sem(sem_t *sem)
{
	if (sem_close(sem))
		ft_error(ERROR_CLOSING);
	sem = NULL;
}

void	ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	t_phi	*iter_next;
	int		nb;

	iter = phi;
	nb = params->nb;
	while (nb-- > 0)
	{
		ft_free_sem(iter->order_start);
		ft_free_sem(iter->order_end);
		ft_free_sem(iter->check);
		ft_free_sem(iter->stop);
		iter->stop = NULL;
		iter_next = iter->next;
		free(iter);
		iter = iter_next;
	}
	iter = NULL;
}

void	ft_free_params(t_params *params)
{
	ft_free_sem(params->has_eaten);
	ft_free_sem(params->game_over);
	ft_free_sem(params->display);
	ft_free_sem(params->forks);
	free(params);
	params = NULL;
}

void	ft_free(t_philo_two *p)
{
	if (p->phi)
		ft_free_philosophers(p->phi, p->params);
	if (p->params)
		ft_free_params(p->params);
	free(p);
	p = NULL;
}
