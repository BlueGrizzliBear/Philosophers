/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/16 15:38:43 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

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
	memset(phi->id, '\0', 13);
	ft_itoa(phi->id, p->params->nb - inv_id);
	phi->status = 1;
	phi->has_eaten = 0;
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
	ph_three->params = p;
	if (!(ph_three->phi = ft_init_phi(ph_three, p->nb, NULL)))
		return (NULL);
	return (ph_three);
}
