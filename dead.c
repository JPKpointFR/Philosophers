/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:32:03 by cisse             #+#    #+#             */
/*   Updated: 2025/03/18 23:40:55 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_one_philo(t_dinner *table, int i, size_t now)
{
	pthread_mutex_lock(&table->check);
	if (!table->dead && (now - table->philos[i].last_meal >= (size_t)table->time_to_die))
	{
		print_status(&table->philos[i], DEAD, 1);
		table->dead = 1;
		pthread_mutex_unlock(&table->check);
		return (1);
	}
	pthread_mutex_unlock(&table->check);
	return (0);
}

static int check_philo_loop(t_dinner *table)
{
	int i;
	size_t now;

	i = 0;
	now = time_now();
	while (i < table->nb_philos)
	{
		if (check_one_philo(table, i, now))
			return (1);
		i++;
	}
	return (0);
}

int check_all_sated(t_dinner *table)
{
	int i;
	int all_sated;

	all_sated = 1;
	if (table->nb_meals == -1)
		return (0);
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i].nb_meal < table->nb_meals)
		{
			all_sated = 0;
			break;
		}
		i++;
	}
	return (all_sated);
}

int check_satiation(t_dinner *table)
{
	int result;

	pthread_mutex_lock(&table->check);
	result = check_all_sated(table);
	if (result)
		table->sated = 1;
	pthread_mutex_unlock(&table->check);
	return (result);
}

void check_death(t_dinner *table)
{
	while (1)
	{
		if (check_philo_loop(table))
			return ;
		if (check_satiation(table))
			return ;
		usleep(100);
	}
}
