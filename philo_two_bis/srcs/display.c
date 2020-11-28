/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/25 16:31:10 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

int		ft_in_order(t_phi *phi)
{
	if (sem_wait(phi->params->order))
		return (ft_error(ERROR_LOCK_SEM));
	if (phi->id_nb != phi->params->order_nb)
	{
		if (sem_post(phi->params->order))
			return (ft_error(ERROR_UNLOCK_SEM));
		return (-1);
	}
	if (sem_post(phi->params->order))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}

int		ft_fill_msg_str(char *msg, char *id, char *str, int size)
{
	int i;

	i = -1;
	while (id[++i])
		msg[size++] = id[i];
	i = -1;
	while (str[++i])
		msg[size++] = str[i];
	msg[size + i] = '\0';
	return (size);
}

int		ft_fill_msg_nb(char *msg, int nb)
{
	int size;
	int cpy;

	size = 0;
	cpy = nb;
	while (cpy > 9)
	{
		cpy = cpy / 10;
		size++;
	}
	cpy = size;
	while (nb > 9)
	{
		msg[size--] = nb % 10 + '0';
		nb = nb / 10;
	}
	msg[size] = nb % 10 + '0';
	cpy++;
	return (cpy);
}

int		ft_display(t_phi *phi, char *str)
{
	struct timeval	now;
	static int		reaper = 0;
	char			msg[64];
	int				size;

	gettimeofday(&now, NULL);
	size = ft_fill_msg_nb(msg, get_timestamp(phi->params->start, now));
	size = ft_fill_msg_str(msg, phi->id, str, size);
	if (sem_wait(phi->params->display))
		return (ft_error(ERROR_LOCK_SEM));
	if (reaper != 0)
		return (-1);
	if (phi->status == 0)
		reaper += 1;
	write(1, (void*)msg, size);
	if (sem_post(phi->params->display))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}