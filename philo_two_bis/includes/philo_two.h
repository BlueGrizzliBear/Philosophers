/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbussier <cbussier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:18:36 by cbussier          #+#    #+#             */
/*   Updated: 2020/11/26 16:32:46 by cbussier         ###   ########lyon.fr   */
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
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

# define ERROR_NB_ARG 0
# define ERROR_ARG_VAL 1
# define ERROR_INIT_STRUCT 2
# define ERROR_STRUCT_CREAT 3
# define ERROR_OPEN_SEM 4
# define ERROR_ALLOC_THREAD 5
# define ERROR_LAUNCH_PHI 6
# define ERROR_CREATE_THREAD 7
# define ERROR_LOCK_SEM 8
# define ERROR_UNLOCK_SEM 9
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
	int					must_eat;
	int					game;
	sem_t				*game_status;
	struct timeval		start;
	sem_t				*display;
	sem_t				*forks;
	int					forks_nb;
	sem_t				*order;
	int					order_nb;
}						t_params;

typedef struct			s_phi
{
	int					id_nb;
	char				id[13];
	int					status;
	int					has_eaten;
	struct timeval		last_meal;
	pthread_t			*thread;
	t_params			*params;
	struct s_phi		*next;
}						t_phi;

typedef struct			s_philo_two
{
	t_params			*params;
	t_phi				*phi;
}						t_philo_two;

int						ft_error(int err);
t_params				*ft_parse(char **argv);
int						ft_atoi(const char *str);
void					ft_putstr(char *str);
t_philo_two				*ft_init(t_params *params);
int						ft_eat_sleep_think(t_phi *phi);
int						unlock_forks(t_phi *phi);
int						ft_display(t_phi *phi, char *str);
int						get_timestamp(struct timeval s, struct timeval t);
int						ft_free(t_philo_two *p);
int						ft_is_dead(t_phi *phi);
int						ft_is_over(t_phi *phi);
char					*ft_strdup(char *str);
int						ft_strlen(char *str);
void					ft_itoa(char *id, int nb);
int						ft_in_order(t_phi *phi);

#endif
