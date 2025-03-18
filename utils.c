/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:08:41 by cisse             #+#    #+#             */
/*   Updated: 2025/03/18 11:49:40 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	time_now(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	waste_time(t_dinner *table, size_t sleep_time)
{
	size_t	t;
	int		dead;

	t = time_now();
	while (1)
	{
		pthread_mutex_lock(&table->check);
		dead = table->dead;
		pthread_mutex_unlock(&table->check);
		if (dead || ((time_now() - t) >= sleep_time))
			break ;
		usleep(100);
	}
}

static int check_one_philo(t_dinner *table, int i, size_t now)
{
    pthread_mutex_lock(&table->check);
    if (now - table->philos[i].last_meal >= (size_t)table->time_to_die)
    {
        table->dead = 1;
        print_status(&table->philos[i], DEAD, 1);
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

int check_all_sated(t_dinner *table, int is_locked)
{
    int i;
    int all_sated = 1;

    if (!is_locked)  // Si le mutex n’est pas verrouillé, on le prend
        pthread_mutex_lock(&table->check);

    if (table->nb_meals == -1)
    {
        if (!is_locked)
            pthread_mutex_unlock(&table->check);
        return (0);
    }

    for (i = 0; i < table->nb_philos; i++)
    {
        if (table->philos[i].nb_meal < table->nb_meals)
        {
            all_sated = 0;
            break;
        }
    }

    if (!is_locked)  // On déverrouille seulement si on l’a verrouillé ici
        pthread_mutex_unlock(&table->check);
    return (all_sated);
}

int check_satiation(t_dinner *table)
{
    int result;

    pthread_mutex_lock(&table->check);  // Verrouillage comme avant
    result = check_all_sated(table, 1);  // On passe 1 pour dire "déjà verrouillé"
    if (result)
        table->sated = 1;
    pthread_mutex_unlock(&table->check);  // Déverrouillage comme avant
    return (result);
}

void check_death(t_dinner *table)
{
    while (1)
    {
        if (check_philo_loop(table))
            return;
        if (check_satiation(table))
            return;
        usleep(100);
    }
}

// void	check_death(t_dinner *t)
// {
// 	int	i;

// 	while (!t->sated)
// 	{
// 		i = -1;
// 		while (!t->dead && ++i < t->nb_philos)
// 		{
// 			pthread_mutex_lock(&t->check);
// 			if ((time_now() - t->philos[i].last_meal) > (size_t)t->time_to_die)
// 			{
// 				print_status(&t->philos[i], DEAD, 1);
// 				t->dead = 1;
// 			}
// 			pthread_mutex_unlock(&t->check);
// 			usleep(100);
// 		}
// 		if (t->dead)
// 			break ;
// 		i = 0;
// 		while ((t->nb_meals != -1) && (i < t->nb_philos)
// 			&& (t->philos[i].nb_meal >= t->nb_meals))
// 			i++;
// 		if (i == t->nb_philos)
// 			t->sated = 1;
// 	}
// }

void	leave_life(t_dinner *table, pthread_t *ids)
{
	int	i;

	i = -1;
	while (++i < table->nb_philos)
		pthread_join(ids[i], NULL);
	i = -1;
	while (++i < table->nb_philos)
		pthread_mutex_destroy(&table->philos[i].fork);
	pthread_mutex_destroy(&table->print);
	pthread_mutex_destroy(&table->check);
	free(table->philos);
	free(ids);
}
