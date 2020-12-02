/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/12/02 12:25:09 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

int			ft_create_phi_sem(t_phi *phi)
{
	sem_unlink("/order_start");
	phi->order_start = sem_open("/order_start", O_CREAT, S_IRWXU, 0);
	if (phi->order_start == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	sem_unlink("/order_end");
	phi->order_end = sem_open("/order_end", O_CREAT, S_IRWXU, 0);
	if (phi->order_end == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	sem_unlink("/check");
	phi->check = sem_open("/check", O_CREAT, S_IRWXU, 1);
	if (phi->check == SEM_FAILED)
		return (ft_error(ERROR_OPEN_SEM));
	return (0);
}

t_phi		*ft_init_phi(t_philo_two *p, int inv_id, t_phi *addr)
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
	if (ft_create_phi_sem(phi))
		return (NULL);
	phi->status = 1;
	phi->has_eaten = 0;
	phi->params = p->params;
	if (inv_id != 1)
		phi->next = ft_init_phi(p, inv_id - 1, addr);
	else
		phi->next = addr;
	return (phi);
}

t_philo_two	*ft_init(t_params *p)
{
	t_philo_two *ph_two;

	if (!(ph_two = malloc(sizeof(t_philo_two))))
	{
		ft_free_params(p);
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	ph_two->params = p;
	if (!(ph_two->phi = ft_init_phi(ph_two, p->nb, NULL)))
	{
		ft_free(ph_two);
		return (NULL);
	}
	return (ph_two);
}
