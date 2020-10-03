/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/03 18:19:41 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

t_fork	*ft_init_forks(int nb, int inv_id, t_fork *addr)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(t_fork))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	if (inv_id == nb)
		addr = fork;
	fork->id = nb - inv_id;
	fork->status = 0;
	if (!(fork->mutex = malloc(sizeof(pthread_mutex_t))))
	{
		ft_error(ERROR_ALLOC_MUTEX);
		return (NULL);
	}
	if (pthread_mutex_init(fork->mutex, NULL))
	{
		ft_error(ERROR_INIT_MUTEX);
		return (NULL);
	}
	if (inv_id != 1)
		fork->next = ft_init_forks(nb, inv_id - 1, addr);
	else
		fork->next = addr;
	return (fork);
}

t_phi	*ft_init_phi(t_fork *f, t_philo_one *p, int inv_id, t_phi *addr)
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
	phi->game = &p->game;
	// phi->alive = 0;
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

void		ft_display_forks(int nb, t_fork *forks)
{
	t_fork *iter;
	int i;

	i = nb;
	iter = forks;
	while (i > -1)
	{
		dprintf(2, "fork#%d addr|%p|\n", iter->id, iter);
		iter = iter->next;
		dprintf(2, "NEXT fork#%d addr|%p|\n", iter->id, iter);
		i--;
	}
}

void		ft_display_phi(int nb, t_phi *phi)
{
	t_phi *iter_p;
	int i;

	i = nb;
	iter_p = phi;
	while (i > -1)
	{
		dprintf(2, "LF|%d|< PHI|%d| > RF|%d|\n", iter_p->left_fork->id, iter_p->id, iter_p->right_fork->id);
		dprintf(2, "phi#%d addr|%p|\n", iter_p->id, iter_p);
		// dprintf(2, "phi->left_fork#%d addr|%p|\n", iter_p->left_fork->id, iter_p->left_fork);
		dprintf(2, "l_fork->mutex addr|%p|\n", iter_p->left_fork->mutex);
		// dprintf(2, "phi->right_fork#%d addr|%p|\n", iter_p->right_fork->id, iter_p->right_fork);
		dprintf(2, "r_fork->mutex addr|%p|\n", iter_p->right_fork->mutex);
		iter_p = iter_p->next;
		i--;
	}

}

t_philo_one	*ft_init(t_params *params)
{
	t_philo_one *philo_one;

	if (!(philo_one = malloc(sizeof(t_philo_one))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	philo_one->game = 1;
	philo_one->params = params;
	if (!(philo_one->forks = ft_init_forks(params->nb, params->nb, NULL)))
		return (NULL);
	if (!(philo_one->phi = ft_init_phi(philo_one->forks, philo_one, params->nb, NULL)))
		return (NULL);

	// ft_display_forks(philo_one->params->nb, philo_one->forks);	
	// dprintf(2, "\n");
	// ft_display_phi(philo_one->params->nb, philo_one->phi);	
	// dprintf(2, "\n");

	return (philo_one);
}