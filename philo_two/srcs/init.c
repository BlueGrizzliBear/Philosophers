/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/21 16:40:16 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

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
	ft_itoa(phi->id, p->params->nb - inv_id);
	phi->status = 1;
	phi->has_eaten = 0;
	if (!(phi->thread = malloc(sizeof(pthread_t))))
	{
		ft_error(ERROR_ALLOC_THREAD);
		return (NULL);
	}
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
		ft_error(ERROR_STRUCT_CREAT);
		return (NULL);
	}
	ph_two->params = p;
	if (!(ph_two->phi = ft_init_phi(ph_two, p->nb, NULL)))
		return (NULL);
	return (ph_two);
}
