/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 12:42:43 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/19 13:27:24 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000
		+ tv.tv_usec / 1000);
}

void	print_status(t_coder *coder, char *message)
{
	long long	time;

	time = get_time_ms() - coder->sim->start_time;
	pthread_mutex_lock(&coder->sim->state_mutex);
	printf("%lld %d %s\n", time, coder->id, message);
	pthread_mutex_unlock(&coder->sim->state_mutex);
}

void	ms_to_timespec(long long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}

int	all_coders_finished(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_count
			< sim->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

int	get_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->state_mutex);
	running = sim->running;
	pthread_mutex_unlock(&sim->state_mutex);
	return (running);
}
