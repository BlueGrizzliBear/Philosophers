/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/06 15:00:04 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int		ft_free_forks(t_fork *f, t_params *params)
{
	t_fork	*iter;
	int		nb;

	nb = params->nb;
	while (nb > 0)
	{
		if (pthread_mutex_destroy(f->mutex))
			ft_error(ERROR_DESTROY);
		free(f->mutex);
		f->mutex = NULL;
		iter = f;
		f = f->next;
		free(iter);
		nb--;
	}
	f = NULL;
	return (0);
}

int		ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	int		nb;

	nb = params->nb;
	while (nb > 0)
	{
		free(phi->thread);
		phi->thread = NULL;
		phi->left_fork = NULL;
		phi->right_fork = NULL;
		iter = phi->next;
		free(phi);
		phi = iter;
		nb--;
	}
	phi = NULL;
	return (0);
}

int		ft_free(t_philo_one *p)
{
	if (ft_free_forks(p->forks, p->params))
		return (1);
	ft_free_philosophers(p->phi, p->params);
	if (pthread_mutex_destroy(p->params->display))
		ft_error(ERROR_DESTROY);
	free(p->params->display);
	p->params->display = NULL;
	free(p->params);
	p->params = NULL;
	free(p);
	p = NULL;
	return (0);
}
