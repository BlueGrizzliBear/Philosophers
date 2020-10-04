/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:43:27 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/04 11:10:32 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

void	ft_arg_error(int err)
{
	if (err == ERROR_NB_ARG)
		ft_putstr("Error: incorrect argument number\n");
	if (err == ERROR_ARG_VAL)
		ft_putstr("Error: incorrect argument value\n");
}

void	ft_mem_error(int err)
{
	if (err == ERROR_INIT_STRUCT)
		ft_putstr("Error: structure could not be initialized\n");
	if (err == ERROR_STRUCT_CREAT)
		ft_putstr("Error: structure could not be created\n");
	if (err == ERROR_OPEN_SEM)
		ft_putstr("Error: could not open semaphore\n");
	if (err == ERROR_ALLOC_THREAD)
		ft_putstr("Error: could not allocate memory to thread\n");
	if (err == ERROR_CREATE_THREAD)
		ft_putstr("Error: could not create thread\n");
}

void	ft_program_error(int err)
{
	if (err == ERROR_LAUNCH_PHI)
		ft_putstr("Error: could not launch philosophers\n");
	if (err == ERROR_LOCK_SEM)
		ft_putstr("Error: could not lock semaphore\n");
	if (err == ERROR_UNLOCK_SEM)
		ft_putstr("Error: could not unlock semaphore\n");
	if (err == ERROR_GTOD)
		ft_putstr("Error: gettimeofday() returned an error\n");
	if (err == ERROR_MEM_ALLOC)
		ft_putstr("Error: could not allocate memory\n");
	if (err == ERROR_DISPLAY)
		ft_putstr("Error: could not display message\n");
	if (err == ERROR_STANDBY)
		ft_putstr("Error: an error occured during ft_standby()\n");
	if (err == ERROR_CLOSING)
		ft_putstr("Error: an error occured while closing semaphore\n");
	if (err == ERROR_JOIN_THREAD)
		ft_putstr("Error: an error occured during thread join\n");
	if (err == ERROR_UNLINK)
		ft_putstr("Error: an error occured while unlinking semaphore\n");
}

int		ft_error(int err)
{
	ft_arg_error(err);
	ft_mem_error(err);
	ft_program_error(err);
	return (1);
}
