/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:17:54 by cisse             #+#    #+#             */
/*   Updated: 2025/03/18 19:24:37 by cisse            ###   ########.fr       */
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

