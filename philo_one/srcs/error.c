/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/03 16:36:37 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

int 	ft_error(int err)
{
	if (err == ERROR_NB_ARG)
		ft_putstr("Error: incorrect argument number\n");
	if (err == ERROR_ARG_VAL)
		ft_putstr("Error: incorrect argument value\n");
	if (err == ERROR_INIT_STRUCT)
		ft_putstr("Error: structure could not be initialized\n");
	if (err == ERROR_STRUCT_CREAT)
		ft_putstr("Error: structure could not be created\n");
	if (err == ERROR_ALLOC_MUTEX)
		ft_putstr("Error: could not allocate memory to mutex\n");
	if (err == ERROR_ALLOC_THREAD)
		ft_putstr("Error: could not allocate memory to thread\n");
	if (err == ERROR_INIT_MUTEX)
		ft_putstr("Error: could not initialise mutex\n");
	if (err == ERROR_LAUNCH_PHI)
		ft_putstr("Error: could not launch philosophers\n");
	if (err == ERROR_CREATE_THREAD)
		ft_putstr("Error: could not create thread\n");
	if (err == ERROR_LOCK_MUTEX)
		ft_putstr("Error: could not lock mutex\n");
	if (err == ERROR_UNLOCK_MUTEX)
		ft_putstr("Error: could not unlock mutex\n");
	if (err == ERROR_GTOD)
		ft_putstr("Error: gettimeofday() returned an error\n");
	if (err == ERROR_MEM_ALLOC)
		ft_putstr("Error: could not allocate memory\n");
	if (err == ERROR_DISPLAY)
		ft_putstr("Error: could not display message\n");
	if (err == ERROR_STANDBY)
		ft_putstr("Error: an error occured during ft_standby()\n");
	if (err == ERROR_DESTROY)
		ft_putstr("Error: an error occured during mutex destruction\n");
	if (err == ERROR_JOIN_THREAD)
		ft_putstr("Error: an error occured during thread join\n");
	return (1);	
}