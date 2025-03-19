/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:05:58 by cisse             #+#    #+#             */
/*   Updated: 2025/03/19 21:16:01 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void assign_forks(t_philo *philo, pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = &philo->fork;
		*second = &philo->next->fork;
	}
	else
	{
		*first = &philo->next->fork;
		*second = &philo->fork;
	}
}

static void lock_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	if (philo->table->nb_philos == 1)
	{
		pthread_mutex_lock(&philo->fork);
		print_status(philo, FORK, 0);
		waste_time(philo->table, philo->table->time_to_die);
		pthread_mutex_lock(&philo->table->check);
		print_status(philo, DEAD, 1);
		philo->table->dead = 1;
		pthread_mutex_unlock(&philo->table->check);
		pthread_mutex_unlock(&philo->fork);
		return ;
	}
	pthread_mutex_lock(first);
	print_status(philo, FORK, 0);
	pthread_mutex_lock(second);
	print_status(philo, FORK, 0);
}

static void eat_and_release(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	t_dinner *table;

	table = philo->table;
	pthread_mutex_lock(&table->check);
	philo->last_meal = time_now();
	philo->nb_meal++;
	pthread_mutex_unlock(&table->check);
	print_status(philo, EAT, 0);
	waste_time(table, table->time_to_eat);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

static void eaten(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	pthread_mutex_lock(&philo->table->check);
	if (philo->table->dead)
	{
		pthread_mutex_unlock(&philo->table->check);
		return ;
	}
	pthread_mutex_unlock(&philo->table->check);
	assign_forks(philo, &first_fork, &second_fork);
	lock_forks(philo, first_fork, second_fork);
	if (philo->table->nb_philos == 1)
		return;
	eat_and_release(philo, first_fork, second_fork);
}

void	*live(void *genius)
{
	t_philo		*philo;
	t_dinner	*table;
	int			stop;

	philo = (t_philo *)genius;
	table = philo->table;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&table->check);
		stop = table->sated || table->dead;
		pthread_mutex_unlock(&table->check);
		if (stop)
			break ;
		eaten(philo);
		print_status(philo, SLEEP, 0);
		waste_time(table, table->time_to_sleep);
		print_status(philo, THINK, 0);
	}
	return (NULL);
}
