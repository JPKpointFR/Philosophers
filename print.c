/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:24:49 by cisse             #+#    #+#             */
/*   Updated: 2025/03/18 19:33:02 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*get_status(int status)
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
