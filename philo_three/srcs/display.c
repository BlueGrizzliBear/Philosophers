/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:59:40 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/16 15:35:42 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_three.h"

// int		ft_get_size(int timestamp, int phi_nb, char *str)
// {
// 	int iter;
// 	int size;
// 	int nb;

// 	size = 0;
// 	nb = timestamp;
// 	while (nb > 9 || nb < -9)
// 	{
// 		nb = nb / 10;
// 		size++;
// 	}
// 	size++;
// 	while (phi_nb > 9 || phi_nb < -9)
// 	{
// 		phi_nb = phi_nb / 10;
// 		size++;
// 	}
// 	size++;
// 	iter = 0;
// 	while (str[iter])
// 	{
// 		iter++;
// 		size++;
// 	}
// 	return (size);
// }

// void	ft_fill_msg_str(char *msg, char *str)
// {
// 	int i;
// 	int len;

// 	i = 0;
// 	len = 0;
// 	while (msg[len])
// 		len++;
// 	while (str[i])
// 	{
// 		msg[len + i] = str[i];
// 		i++;
// 	}
// 	msg[len + i] = '\0';
// }

// void	ft_fill_msg_nb(char *msg, int nb)
// {
// 	int size;
// 	int a;
// 	int len;

// 	len = 0;
// 	while (msg[len])
// 		len++;
// 	size = 0;
// 	a = nb;
// 	while (a > 9)
// 	{
// 		a = a / 10;
// 		size++;
// 	}
// 	size++;
// 	msg[len + size] = ' ';
// 	size--;
// 	while (nb > 9)
// 	{
// 		msg[len + size] = nb % 10 + '0';
// 		nb = nb / 10;
// 		size--;
// 	}
// 	msg[len + size] = nb % 10 + '0';
// }

// int		ft_build_msg(t_phi *phi, int timestamp, char *str, int size)
// {
// 	char	msg[size];
// 	char	*m;

// 	m = memset((void*)msg, '\0', size);
// 	ft_fill_msg_nb(m, timestamp);
// 	ft_fill_msg_nb(m, phi->id);
// 	ft_fill_msg_str(m, str);
// 	if (sem_wait(phi->params->display))
// 		return (ft_error(ERROR_LOCK_SEM));
// 	write(1, m, ft_strlen(m));
// 	if (sem_post(phi->params->display))
// 		return (ft_error(ERROR_UNLOCK_SEM));
// 	return (0);
// }

// int		ft_display(t_phi *phi, char *str)
// {
// 	struct timeval	now;
// 	int				timestamp;
// 	int				size;

// 	if (sem_wait(phi->params->reaper))
// 		return (ft_error(ERROR_LOCK_SEM));
// 	if (gettimeofday(&now, NULL))
// 		return (ft_error(ERROR_GTOD));
// 	timestamp = ft_get_timestamp(phi->params->start, now);
// 	size = (ft_get_size(timestamp, phi->id, str) + 3);
// 	ft_build_msg(phi, timestamp, str, size);
// 	if (phi->status == 0)
// 		return (-1);
// 	if (sem_post(phi->params->reaper))
// 		return (ft_error(ERROR_LOCK_SEM));
// 	return (0);
// }

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

int		ft_build_msg(t_phi *phi, int timestamp, char *str)
{
	char	msg[64];
	int		size;

	size = ft_fill_msg_nb(msg, timestamp);
	size = ft_fill_msg_str(msg, phi->id, str, size);
	if (sem_wait(phi->params->display))
		return (ft_error(ERROR_LOCK_SEM));
	write(1, (void*)msg, size);
	if (sem_post(phi->params->display))
		return (ft_error(ERROR_UNLOCK_SEM));
	return (0);
}

int		ft_display(t_phi *phi, char *str)
{
	struct timeval	now;
	int				timestamp;
	static int		reaper = 0;

	if (reaper != 0)
		return (-1);
	if (phi->status == 0)
		reaper += 1;
	gettimeofday(&now, NULL);
	timestamp = ft_get_timestamp(phi->params->start, now);
	ft_build_msg(phi, timestamp, str);
	return (0);
}
