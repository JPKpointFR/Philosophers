/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:32:03 by cisse             #+#    #+#             */
/*   Updated: 2025/03/19 22:06:24 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	check_death(t_dinner *table)
// {
// 	int	i;
// 	int	dead;

// 	while (!table->sated)
// 	{
// 		i = -1;
// 		pthread_mutex_lock(&table->check);
// 		dead = !table->dead;
// 		pthread_mutex_unlock(&table->check);
// 		while (dead && ++i < table->nb_philos)
// 		{
// 			pthread_mutex_lock(&table->check);
// 			if ((time_now() - table->philos[i].last_meal) > (size_t)table->time_to_die)
// 			{
// 				print_status(&table->philos[i], DEAD, 1);
// 				table->dead = 1;
// 			}
// 			pthread_mutex_unlock(&table->check);
// 			usleep(100);
// 			pthread_mutex_lock(&table->check);
// 			dead = !table->dead;
// 			pthread_mutex_unlock(&table->check);
// 		}
// 		pthread_mutex_lock(&table->check);
// 		dead = table->dead;
// 		pthread_mutex_unlock(&table->check);
// 		if (dead)
// 			break ;
// 		i = 0;
// 		pthread_mutex_lock(&table->check);
// 		dead = (table->philos[i].nb_meal >= table->nb_meals);
// 		pthread_mutex_unlock(&table->check);
// 		while ((table->nb_meals != -1) && (i < table->nb_philos) && dead)
// 		{
// 			pthread_mutex_lock(&table->check);
// 			dead = (table->philos[i].nb_meal >= table->nb_meals);
// 			pthread_mutex_unlock(&table->check);
// 			i++;
// 		}
// 		if (i == table->nb_philos)
// 		{
// 			pthread_mutex_lock(&table->check);
// 			table->sated = 1;
// 			pthread_mutex_unlock(&table->check);
// 		}
// 	}
// }

static int	check_philo_death(t_dinner *table)
{
	int	i;
	int	dead;

	i = -1;
	pthread_mutex_lock(&table->check);
	dead = !table->dead;
	pthread_mutex_unlock(&table->check);
	while (dead && ++i < table->nb_philos)
	{
		pthread_mutex_lock(&table->check);
		if ((time_now() - table->philos[i].last_meal) > (size_t)table->time_to_die)
		{
			print_status(&table->philos[i], DEAD, 1);
			table->dead = 1;
		}
		pthread_mutex_unlock(&table->check);
		usleep(100);
		pthread_mutex_lock(&table->check);
		dead = !table->dead;
		pthread_mutex_unlock(&table->check);
	}
	return (table->dead);
}

static void	check_philo_sated(t_dinner *table)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&table->check);
	while ((table->nb_meals != -1) && (i < table->nb_philos) &&
		(table->philos[i].nb_meal >= table->nb_meals))
	{
		i++;
	}
	if (i == table->nb_philos)
		table->sated = 1;
	pthread_mutex_unlock(&table->check);
}

void	check_death(t_dinner *table)
{
	int	sated;
	pthread_mutex_lock(&table->check);
	sated = !table->sated;
	pthread_mutex_unlock(&table->check);
	while (sated)
	{
		if (check_philo_death(table))
			break;
		pthread_mutex_lock(&table->check);
		if (table->dead)
			break ;
		pthread_mutex_unlock(&table->check);
		check_philo_sated(table);
		pthread_mutex_lock(&table->check);
		sated = !table->sated;
		pthread_mutex_unlock(&table->check);
	}
}
