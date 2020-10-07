/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 19:35:01 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

t_fork		*ft_init_forks(int nb, int inv_id, t_fork *addr)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(t_fork))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	if (inv_id == nb)
		addr = fork;
	fork->id = inv_id - nb;
	fork->status = 0;
	if (!(fork->mutex = malloc(sizeof(pthread_mutex_t))) ||
	(pthread_mutex_init(fork->mutex, NULL)))
	{
		ft_error(ERROR_ALLOC_INIT_MUTEX);
		return (NULL);
	}
	if (inv_id != 1)
		fork->next = ft_init_forks(nb, inv_id - 1, addr);
	else
		fork->next = addr;
	return (fork);
}

t_phi		*ft_init_phi(t_fork *f, t_philo_one *p, int inv_id, t_phi *addr)
{
	t_phi *phi;

	if (!(phi = malloc(sizeof(t_phi))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	if (inv_id == p->params->nb)
		addr = phi;
	phi->id = p->params->nb - inv_id;
	phi->status = 1;
	phi->has_eaten = 0;
	if (!(phi->thread = malloc(sizeof(pthread_t))))
	{
		ft_error(ERROR_ALLOC_THREAD);
		return (NULL);
	}
	phi->params = p->params;
	phi->left_fork = f;
	phi->right_fork = f->next;
	if (inv_id != 1)
		phi->next = ft_init_phi(f->next, p, inv_id - 1, addr);
	else
		phi->next = addr;
	return (phi);
}

t_philo_one	*ft_init(t_params *p)
{
	t_philo_one *ph_one;

	if (!(ph_one = malloc(sizeof(t_philo_one))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	ph_one->params = p;
	if (!(ph_one->forks = ft_init_forks(p->nb, p->nb, NULL)))
		return (NULL);
	if (!(ph_one->phi = ft_init_phi(ph_one->forks, ph_one, p->nb, NULL)))
		return (NULL);
	return (ph_one);
}
