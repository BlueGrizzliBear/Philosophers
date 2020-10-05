/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/05 12:09:58 by cbussier         ###   ########lyon.fr   */
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
