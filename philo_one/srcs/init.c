/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 11:17:00 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int			ft_create_phi_mutex(t_phi *phi)
{
	if (!(phi->check = ft_create_mutex()))
		return (1);
	if (!(phi->order = ft_create_mutex()))
		return (1);
	if (pthread_mutex_lock(phi->order))
	{
		ft_error(ERROR_LOCK_MUTEX);
		return (1);
	}
	return (0);
}

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
	if (!(fork->mutex = ft_create_mutex()))
		return (NULL);
	if (inv_id != 1)
		fork->next = ft_init_forks(nb, inv_id - 1, addr);
	else
		fork->next = addr;
	return (fork);
}

t_phi		*ft_init_phi(t_philo_one *p, int inv_id, t_phi *addr, t_fork *f)
{
	t_phi *phi;

	if (!(phi = malloc(sizeof(t_phi))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	if (inv_id == p->params->nb)
		addr = phi;
	phi->id_nb = p->params->nb - inv_id;
	memset(phi->id, '\0', 13);
	ft_itoa(phi->id, p->params->nb - inv_id + 1);
	if (ft_create_phi_mutex(phi))
		return (NULL);
	phi->left_fork = f;
	phi->right_fork = f->next;
	phi->status = 1;
	phi->has_eaten = 0;
	phi->ordo = 1;
	phi->params = p->params;
	if (inv_id != 1)
		phi->next = ft_init_phi(p, inv_id - 1, addr, f->next);
	else
		phi->next = addr;
	return (phi);
}

t_philo_one	*ft_init(t_params *p)
{
	t_philo_one *ph_one;

	if (!(ph_one = malloc(sizeof(t_philo_one))))
	{
		ft_free_params(p);
		exit(ft_error(ERROR_STRUCT_CREAT));
	}
	ph_one->params = p;
	if (!(ph_one->forks = ft_init_forks(p->nb, p->nb, NULL)))
		return (NULL);
	if (!(ph_one->phi = ft_init_phi(ph_one, p->nb, NULL, ph_one->forks)))
	{
		ft_free(ph_one);
		return (NULL);
	}
	return (ph_one);
}
