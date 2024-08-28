/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 23:11:19 by saleshin          #+#    #+#             */
/*   Updated: 2024/07/14 23:11:32 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_philo *philosophers, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		philosophers[i].id = i + 1;
		philosophers[i].args = args;
		philosophers[i].eat_count = 0;
		philosophers[i].timer_life = args->time_to_die;
		pthread_mutex_init(&philosophers[i].fork, NULL);
		pthread_mutex_init(&philosophers[i].timer_mutex, NULL);
		philosophers[i].next_fork = &philosophers[(i + 1)
			% args->number_of_philosophers].fork;
		i++;
	}
	return (0);
}

int	init_threads(t_philo *philosophers, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		if (pthread_create(&philosophers[i].thread_philo,
				NULL, philo_routine, &philosophers[i]))
			return (i);
		i++;
	}
	return (-1);
}

int	one_philo(t_args *args)
{
	if (args->number_of_philosophers == 1)
	{
		printf("%d 1 has taken a fork\n", 0);
		usleep(args->time_to_die * 1000);
		printf("%d 1 died\n", args->time_to_die);
		return (1);
	}
	return (0);
}

int	finish_threads(t_philo *philosophers, t_args *args, int f)
{
	int	i;

	i = 0;
	while (i < f)
		pthread_join(philosophers[i++].thread_philo, NULL);
	i = 0;
	while (i < f)
	{
		pthread_mutex_destroy(&philosophers[i].fork);
		pthread_mutex_destroy(&philosophers[i].timer_mutex);
		i++;
	}
	pthread_mutex_destroy(&args->mutex_global);
	pthread_mutex_destroy(&args->mutex_print);
	pthread_mutex_destroy(&args->mutex_end);
	pthread_mutex_destroy(&args->mutex_eat);
	free(philosophers);
	free(args);
	return (0);
}
