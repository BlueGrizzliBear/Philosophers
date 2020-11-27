/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/27 11:33:50 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

int			ft_init_phi_sem(t_phi *phi)
{
	sem_unlink("/order");
	phi->order = sem_open("/order", O_CREAT, S_IRWXU, 1);
	if (phi->order == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	if (sem_wait(phi->order))
		return (ft_error(ERROR_LOCK_SEM));
	sem_unlink("/check");
	phi->check = sem_open("/check", O_CREAT, S_IRWXU, 1);
	if (phi->check == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	sem_unlink("/stop");
	phi->stop = sem_open("/stop", O_CREAT, S_IRWXU, 0);
	if (phi->stop == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	return (0);
}

t_phi			*ft_init_phi(t_philo_three *p, int inv_id, t_phi *addr)
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
	ft_itoa(phi->id, p->params->nb - inv_id);
	if (ft_init_phi_sem(phi))
		return (NULL);
	phi->status = 1;
	phi->has_eaten = 0;
	// if (!(phi->brain = malloc(sizeof(pthread_t))))
	// {
	// 	ft_error(ERROR_MEM_ALLOC);
	// 	return (NULL);
	// }
	phi->params = p->params;
	if (inv_id != 1)
		phi->next = ft_init_phi(p, inv_id - 1, addr);
	else
		phi->next = addr;
	return (phi);
}

t_philo_three	*ft_init(t_params *p)
{
	t_philo_three *ph_three;

	if (!(ph_three = malloc(sizeof(t_philo_three))))
	{
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	// if (!(ph_three->thread = malloc(sizeof(pthread_t))))
	// {
	// 	ft_error(ERROR_MEM_ALLOC);
	// 	return (NULL);
	// }
	ph_three->params = p;
	if (!(ph_three->phi = ft_init_phi(ph_three, p->nb, NULL)))
		return (NULL);
	return (ph_three);
}
