/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 11:19:16 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/28 11:19:18 by cbussier         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>

# define ERROR_NB_ARG 0
# define ERROR_ARG_VAL 1
# define ERROR_INIT_STRUCT 2
# define ERROR_STRUCT_CREAT 3
# define ERROR_OPEN_SEM 4
# define ERROR_LAUNCH_PHI 5
# define ERROR_CREATE_FORK 6
# define ERROR_CREATE_THREAD 7
# define ERROR_LOCK_SEM 8
# define ERROR_UNLOCK_SEM 9
# define ERROR_MEM_ALLOC 10
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
	int					must_eat;
	int					game;
	struct timeval		start;
	sem_t				*has_eaten;
	sem_t				*game_over;
	sem_t				*display;
	sem_t				*forks;
}						t_params;

typedef struct			s_phi
{
	int					id_nb;
	char				id[13];
	int					status;
	int					has_eaten;
	struct timeval		last_meal;
	pthread_t			entity;
	pthread_t			brain;
	sem_t				*order_start;
	sem_t				*order_end;
	sem_t				*check;
	sem_t				*stop;
	t_params			*params;
	struct s_phi		*next;
}						t_phi;

typedef struct			s_philo_two
{
	pthread_t			ordering;
	pthread_t			has_eaten;
	t_params			*params;
	t_phi				*phi;
}						t_philo_two;

int						ft_error(int err);
t_params				*ft_parse(char **argv);
t_philo_two				*ft_init(t_params *params);

int						ft_strlen(char *str);
void					ft_itoa(char *id, int nb);
int						ft_atoi(const char *str);
void					ft_putstr(char *str);

void					*th_is_alive(void *arg);
int						get_timestamp(struct timeval s, struct timeval t);

int						ft_is_dead(t_phi *phi);
int						ft_is_over(t_phi *phi);

int						ft_display(t_phi *phi, char *str, int last);
int						ft_eat(t_phi *phi);
int						ft_standby(int time);

void					ft_free_params(t_params *params);
void					ft_free(t_philo_two *p);

#endif
