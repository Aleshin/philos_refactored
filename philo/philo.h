/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 23:14:49 by saleshin          #+#    #+#             */
/*   Updated: 2024/07/14 23:15:01 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct args
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				start_time;
	int				number_of_times_each_philosopher_must_eat;
	int				philos_finished;
	int				end;
	pthread_mutex_t	mutex_global;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_end;
	pthread_mutex_t	mutex_eat;
}	t_args;

typedef struct philo
{
	int				id;
	pthread_mutex_t	fork;
	pthread_mutex_t	*next_fork;
	pthread_mutex_t	timer_mutex;
	pthread_t		thread_philo;
	int				timer_life;
	int				eat_count;
	t_args			*args;
}	t_philo;

int			f_time(void);
void		ft_usleep(int millisec);
int			check_args(int argc, char **argv);
unsigned	int	ft_atoi(char *str);
int			print_status(t_philo *philo, char *status);
int			init_args(int argc, char **argv, t_args *args);
int			init_philo(t_philo *philosophers, t_args *args);
int			one_philo(t_args *args);
int			init_threads(t_philo *philosophers, t_args *args);
int			finish_threads(t_philo *philosophers, t_args *args, int f);
void		*philo_routine(void *data);
void		monitor(t_philo *philo);
int			thread_errors(t_philo *philo, t_args *args, int f);

#endif
