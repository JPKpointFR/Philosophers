/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisse <cisse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:52:17 by cisse             #+#    #+#             */
/*   Updated: 2025/03/13 04:26:26 by cisse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_number(int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (++i < argc)
	{
		j = -1;
		if (argv[i][0] == '-')
			return (1);
		else if (argv[i][0] == '+')
			j++;
		if (!ft_isdigit(argv[i][j + 1]))
			return (1);
		while (argv[i][++j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (1);
		}
	}
	return (0);
}

static int	is_valid_int(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (*s == '+' && s++)
		len--;
	while (*s == '0' && *s && len--)
		s++;
	if (len > 10)
		return (1);
	else if (len < 10)
		return (0);
	if (ft_strncmp(s, "2147483648", 10) >= 0)
		return (1);
	return (0);
}

static int	not_good_args(int argc, char **argv)
{
	int		i;
	char	*zo;

	zo = "0\0";
	if (argc == 6)
	{
		if (ft_strncmp(argv[5], zo, 2) == 0)
			return (1);
	}
	if (is_valid_number(argc, argv))
		return (1);
	i = 0;
	while (++i < argc)
	{
		if (is_valid_int(argv[i]))
			return (1);
	}
	return (0);
}

void	init_philos(t_dinner *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].nb_meal = 0;
		table->philos[i].table = table;
		if (i == table->nb_philos - 1)
			table->philos[i].next = &table->philos[0];
		else
			table->philos[i].next = &table->philos[i + 1];
		if (i == 0)
			table->philos[i].prev = &table->philos[table->nb_philos - 1];
		else
			table->philos[i].prev = &table->philos[i - 1];
		pthread_mutex_init(&table->philos[i].fork, NULL);
	}
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->check, NULL);
}

int	check_args(int argc, char **argv, t_dinner *table)
{
	if (not_good_args(argc, argv))
		return (1);
	table->nb_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->nb_meals = -1;
	if (argc == 6)
		table->nb_meals = ft_atoi(argv[5]);
	table->dead = 0;
	table->sated = 0;
	table->start_time = time_now();
	table->philos = malloc(table->nb_philos * sizeof(t_philo));
	if (!table->philos)
		return (1);
	init_philos(table);
	return (0);
}
