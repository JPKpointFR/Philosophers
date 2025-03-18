/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:48:40 by cisse             #+#    #+#             */
/*   Updated: 2025/03/13 03:47:03 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int			i;
	pthread_t	*ids;
	t_dinner	table;

	if (!(argc == 5 || argc == 6) || check_args(argc, argv, &table))
		return (printf("error: args\n"));
	ids = malloc(table.nb_philos * sizeof(pthread_t));
	i = -1;
	while (++i < table.nb_philos)
	{
		if (pthread_create(&ids[i], NULL, &live, &table.philos[i]))
		{
			printf("error: pthread_create()");
			free(table.philos);
			free(ids);
			return (667);
		}
		pthread_mutex_lock(&table.check);
		table.philos[i].last_meal = table.start_time;
		pthread_mutex_unlock(&table.check);
	}
	check_death(&table);
	leave_life(&table, ids);
	return (0);
}
// valgrind --tool=helgrind
