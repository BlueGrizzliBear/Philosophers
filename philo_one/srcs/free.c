/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 09:51:09 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

void	ft_free_mutex(pthread_mutex_t *f)
{
	int ret;

	if ((ret = pthread_mutex_destroy(f)))
		ft_error(ERROR_DESTROY);
	free(f);
	f = NULL;
}

int		ft_free_forks(t_fork *f, t_params *params)
{
	t_fork	*iter;
	int		nb;

	nb = params->nb;
	while (nb > 0)
	{
		dprintf(2, "fork mutex\n");
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

void	ft_free_philosophers(t_phi *phi, t_params *params)
{
	t_phi	*iter;
	t_phi	*iter_next;
	int		nb;

	iter = phi;
	nb = params->nb;
	while (nb-- > 0)
	{
		dprintf(2, "check mutex\n");
		ft_free_mutex(iter->check);
		dprintf(2, "order mutex\n");
		ft_free_mutex(iter->order);
		iter->left_fork = NULL;
		iter->right_fork = NULL;
		iter_next = iter->next;
		free(iter);
		iter = iter_next;
	}
	iter = NULL;
}

void	ft_free_params(t_params *params)
{
	dprintf(2, "display mutex\n");
	ft_free_mutex(params->display);
	free(params);
	params = NULL;
}

void	ft_free(t_philo_one *p)
{
	if (p->phi)
		ft_free_philosophers(p->phi, p->params);
	if (p->forks)
		ft_free_forks(p->forks, p->params);
	if (p->params)
		ft_free_params(p->params);
	free(p);
	p = NULL;
}
