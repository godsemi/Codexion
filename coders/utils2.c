/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:26:54 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 16:29:22 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_running(t_sim *sim, int value)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->running = value;
	pthread_mutex_unlock(&sim->state_mutex);
}

static int	coder_has_burned_out(t_coder *coder)
{
	long long	now;
	long long	deadline;

	now = get_time_ms();
	pthread_mutex_lock(&coder->sim->state_mutex);
	deadline = coder->last_compile_start
		+ coder->sim->time_to_burnout;
	pthread_mutex_unlock(&coder->sim->state_mutex);
	return (now >= deadline);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (get_running(sim))
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			if (!get_running(sim))
				break ;
			if (coder_has_burned_out(&sim->coders[i]))
			{
				print_status(&sim->coders[i], "burned out");
				set_running(sim, 0);
				pthread_cond_broadcast(&sim->dongle_cond);
				pthread_cond_broadcast(&sim->state_cond);
				break ;
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (coder->sim->running)
	{
		if (!take_dongles(coder))
			break ;
		compile(coder);
		if (!coder->sim->running)
		{
			release_dongles(coder);
			break ;
		}
		release_dongles(coder);
		if (!coder_wait(coder, coder->sim->time_to_debug,
				"is debugging"))
			break ;
		if (!coder_wait(coder, coder->sim->time_to_refactor,
				"is refactoring"))
			break ;
	}
	return (NULL);
}

void	freer(t_sim *sim)
{
	free(sim->dongles);
	free(sim->coders);
	sim->dongles = NULL;
	sim->coders = NULL;
}
