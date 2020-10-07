/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:18:36 by cbussier          #+#    #+#             */
/*   Updated: 2020/10/07 12:51:46 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define ERROR_NB_ARG 0
# define ERROR_ARG_VAL 1
# define ERROR_INIT_STRUCT 2
# define ERROR_STRUCT_CREAT 3
# define ERROR_ALLOC_INIT_MUTEX 4
# define ERROR_ALLOC_THREAD 5
# define ERROR_LAUNCH_PHI 6
# define ERROR_CREATE_THREAD 7
# define ERROR_LOCK_MUTEX 8
# define ERROR_UNLOCK_MUTEX 9
# define ERROR_GTOD 10
# define ERROR_MEM_ALLOC 11
# define ERROR_DISPLAY 12
# define ERROR_STANDBY 13
# define ERROR_DESTROY 14
# define ERROR_JOIN_THREAD 15

typedef struct			s_params
{
	int					nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_time_phi_must_eat;
	int					game;
	pthread_mutex_t		*available;
	pthread_mutex_t		*display;
	struct timeval		start;
}						t_params;

typedef struct			s_fork
{
	int					id;
	int					status;
	pthread_mutex_t		*mutex;
	struct s_fork		*next;
}						t_fork;

typedef struct			s_phi
{
	int					id;
	int					status;
	int					has_eaten;
	struct timeval		last_meal;
	pthread_t			*thread;
	t_params			*params;
	t_fork				*left_fork;
	t_fork				*right_fork;
	struct s_phi		*next;
}						t_phi;

typedef struct			s_philo_one
{
	t_params			*params;
	t_fork				*forks;
	t_phi				*phi;
}						t_philo_one;

int						ft_error(int err);
t_params				*ft_parse(char **argv);
int						ft_atoi(const char *str);
void					ft_putstr(char *str);
t_philo_one				*ft_init(t_params *params);
void					*ft_is_alive(void *arg);
int						ft_display(t_phi *phi, char *str);
int						ft_get_timestamp(struct timeval s, struct timeval t);
int						ft_free(t_philo_one *p);
int						ft_is_dead(t_phi *phi);

#endif
