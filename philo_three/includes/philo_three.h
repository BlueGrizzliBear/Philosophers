/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 12:10:57 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/06 20:55:03 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <fcntl.h>
# include <semaphore.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>

# define ERROR_NB_ARG 0
# define ERROR_ARG_VAL 1
# define ERROR_INIT_STRUCT 2
# define ERROR_STRUCT_CREAT 3
# define ERROR_OPEN_SEM 4
# define ERROR_LAUNCH_PHI 5
# define ERROR_CREATE_FORK 6
# define ERROR_LOCK_SEM 7
# define ERROR_UNLOCK_SEM 8
# define ERROR_GTOD 9
# define ERROR_MEM_ALLOC 10
# define ERROR_DISPLAY 11
# define ERROR_STANDBY 12
# define ERROR_CLOSING 13
# define ERROR_JOIN_THREAD 14
# define ERROR_UNLINK 15

typedef struct			s_params
{
	int					nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_time_phi_must_eat;
	// sem_t				*g;
	int					game;
	sem_t				*display;
	int					display_nb;
	sem_t				*forks;
	int					forks_nb;
}						t_params;

typedef struct			s_phi
{
	int					id;
	int					status;
	// int					*game;
	int					has_eaten;
	struct timeval		start;
	struct timeval		last_meal;
	int					pid;
	t_params			*params;
	struct s_phi		*next;
}						t_phi;

typedef struct			s_philo_three
{
	t_params			*params;
	t_phi				*phi;
}						t_philo_three;

int						ft_error(int err);
t_params				*ft_parse(char **argv);
int						ft_atoi(const char *str);
void					ft_putstr(char *str);
t_philo_three			*ft_init(t_params *params);
int						ft_is_alive(void *arg);
int						ft_display(t_phi *phi, char *str);
int						ft_get_timestamp(struct timeval s, struct timeval t);
int						ft_free(t_philo_three *p);
int						ft_is_dead(t_phi *phi);

#endif
