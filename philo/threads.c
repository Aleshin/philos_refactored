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

void	lifecircle(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(philo->next_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->timer_mutex);
	philo->timer_life = f_time() - philo->args->start_time
		+ philo->args->time_to_die;
	pthread_mutex_unlock(&philo->timer_mutex);
	print_status(philo, "is eating");
	ft_usleep(philo->args->time_to_eat);
	pthread_mutex_unlock(philo->next_fork);
	pthread_mutex_unlock(&philo->fork);
	philo->eat_count++;
	if (philo->eat_count
		== philo->args->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&philo->args->mutex_eat);
		philo->args->philos_finished++;
		pthread_mutex_unlock(&philo->args->mutex_eat);
	}
	print_status(philo, "is sleeping");
	ft_usleep(philo->args->time_to_sleep);
	print_status(philo, "is thinking");
}

void	*philo_routine(void *data)
{
	t_philo	*philo;
	int		end;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->args->mutex_global);
	pthread_mutex_unlock(&philo->args->mutex_global);
	if (philo->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(&philo->args->mutex_end);
	end = philo->args->end;
	pthread_mutex_unlock(&philo->args->mutex_end);
	while (!end)
	{
		lifecircle(philo);
		pthread_mutex_lock(&philo->args->mutex_end);
		end = philo->args->end;
		pthread_mutex_unlock(&philo->args->mutex_end);
	}
	return ((void *)0);
}

int	finish(t_args *args)
{
	int	var;

	pthread_mutex_lock(&args->mutex_eat);
	var = args->philos_finished;
	pthread_mutex_unlock(&args->mutex_eat);
	if (var == args->number_of_philosophers)
	{
		pthread_mutex_lock(&args->mutex_end);
		args->end = 1;
		pthread_mutex_unlock(&args->mutex_end);
		return (1);
	}
	return (0);
}

void	monitor(t_philo *philo)
{
	int	i;
	int	var;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&philo[i].timer_mutex);
		var = philo[i].timer_life;
		pthread_mutex_unlock(&philo[i].timer_mutex);
		if (var <= f_time() - philo->args->start_time)
		{
			print_status(&philo[i], "died");
			pthread_mutex_lock(&philo->args->mutex_end);
			philo->args->end = 1;
			pthread_mutex_unlock(&philo->args->mutex_end);
			break ;
		}
		if (finish(philo->args))
			break ;
		i = (i + 1) % philo->args->number_of_philosophers;
	}
}

int	thread_errors(t_philo *philo, t_args *args, int f)
{
	if (f != -1)
	{
		printf ("Threads creating error\n");
		args->end++;
		pthread_mutex_unlock(&philo->args->mutex_global);
		finish_threads(philo, args, f);
		return (0);
	}
	args->start_time = f_time();
	pthread_mutex_unlock(&philo->args->mutex_global);
	return (1);
}
