/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:48:18 by cisse             #+#    #+#             */
/*   Updated: 2025/03/17 17:49:33 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef enum e_status
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
}	t_status;

typedef struct s_philo
{
	int				id;
	int				nb_meal;
	size_t			last_meal;
	pthread_mutex_t	fork;
	struct s_philo	*next;
	struct s_philo	*prev;
	struct s_dinner	*table;
}	t_philo;

typedef struct s_dinner
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	int				nb_philos;
	int				sated;
	int				dead;
	size_t			start_time;
	t_philo			*philos;
	pthread_mutex_t	print;
	pthread_mutex_t	check;
}	t_dinner;

int		ft_atoi(const char *str);
int		ft_isdigit( int c );
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		check_args(int argc, char **argv, t_dinner *table);
void	init_philos(t_dinner *table);
size_t	time_now(void);
void	waste_time(t_dinner *table, size_t sleep_time);
void	leave_life(t_dinner *table, pthread_t *ids);
void	check_death(t_dinner *table);
void	*live(void *philo);
void	print_status(t_philo *philo, int status, int is_locked);
