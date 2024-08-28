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

void	init_params(char **argv, t_args *args)
{
	args->number_of_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->number_of_times_each_philosopher_must_eat = 0;
	args->end = 0;
	args->philos_finished = 0;
	pthread_mutex_init(&args->mutex_global, NULL);
	pthread_mutex_init(&args->mutex_end, NULL);
	pthread_mutex_init(&args->mutex_eat, NULL);
	pthread_mutex_init(&args->mutex_print, NULL);
}

int	init_args(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
	{
		printf("Wrong # of args! Need:\n"
			"number_of_philosophers\n"
			"time_to_die\n"
			"time_to_eat\n"
			"time_to_sleep\n"
			"<number_of_times_each_philosopher_must_eat>\n");
		return (1);
	}
	init_params(argv, args);
	if (check_args(argc, argv))
		return (1);
	if (argc == 6)
		args->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	t_args		*args;
	int			f;

	args = malloc(sizeof(t_args));
	if (!args)
		return (1);
	if (init_args(argc, argv, args) || one_philo(args))
	{
		free(args);
		return (1);
	}
	philos = malloc(args->number_of_philosophers * sizeof(t_philo));
	if (!philos)
		return (1);
	memset(philos, 0, sizeof(t_philo) * (args->number_of_philosophers));
	pthread_mutex_lock(&args->mutex_global);
	init_philo(philos, args);
	f = init_threads(philos, args);
	if (thread_errors(philos, args, f))
	{
		monitor(philos);
		finish_threads(philos, args, args->number_of_philosophers);
	}
	return (0);
}
