/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:05:58 by cisse             #+#    #+#             */
/*   Updated: 2025/03/18 11:23:03 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*get_status(int status)
{
	if (status == FORK)
		return ("🍴 has taken a fork");
	if (status == EAT)
		return ("🍽 is eating");
	if (status == SLEEP)
		return ("😴 is sleeping");
	if (status == THINK)
		return ("🤔 is thinking");
	if (status == DEAD)
		return ("💀 died");
	return (NULL);
}

void	print_status(t_philo *philo, int status, int is_locked)
{
	int		should_print;
	size_t	t;

	if (!is_locked)
		pthread_mutex_lock(&philo->table->check);
	should_print = (!philo->table->dead && !philo->table->sated);
	if (should_print)
		t = time_now() - philo->table->start_time;
	if (!is_locked)
		pthread_mutex_unlock(&philo->table->check);
	if (should_print)
	{
		pthread_mutex_lock(&philo->table->print);
		printf("%ld %d %s\n", t, philo->id, get_status(status));
		pthread_mutex_unlock(&philo->table->print);
	}
}

// static void	eaten(t_philo	*philo)
// {
// 	t_dinner	*table;

// 	table = philo->table;
// 	pthread_mutex_lock(&philo->fork);
// 	print_status(philo, FORK, 0);
// 	if (philo->table->nb_philos == 1)
// 	{
// 		waste_time(table, table->time_to_die);
// 		print_status(philo, DEAD, 0);
// 		pthread_mutex_unlock(&philo->fork);
// 		table->dead = 1;
// 		return ;
// 	}
// 	pthread_mutex_lock(&philo->next->fork);
// 	print_status(philo, FORK, 0);
// 	pthread_mutex_lock(&table->check);
// 	philo->nb_meal++;
// 	print_status(philo, EAT, 1);
// 	philo->last_meal = time_now();
// 	pthread_mutex_unlock(&table->check);
// 	waste_time(table, table->time_to_eat);
// 	pthread_mutex_unlock(&philo->fork);
// 	pthread_mutex_unlock(&philo->next->fork);
// }

static void assign_forks(t_philo *philo, pthread_mutex_t **first, pthread_mutex_t **second)
{
    if (&philo->fork < &philo->next->fork)
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
        print_status(philo, DEAD, 0);
        pthread_mutex_unlock(&philo->fork);
        philo->table->dead = 1;
        return;
    }
    pthread_mutex_lock(first);
    print_status(philo, FORK, 0);
    pthread_mutex_lock(second);
    print_status(philo, FORK, 0);
}

static void eat_and_release(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
    t_dinner *table = philo->table;

    pthread_mutex_lock(&table->check);
    philo->nb_meal++;
    philo->last_meal = time_now();
    print_status(philo, EAT, 1);
    pthread_mutex_unlock(&table->check);
    waste_time(table, table->time_to_eat);
    pthread_mutex_unlock(second);
    pthread_mutex_unlock(first);
}

static void eaten(t_philo *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

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
