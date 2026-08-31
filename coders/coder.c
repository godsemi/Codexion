/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 16:18:45 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/24 17:06:18 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_wait(t_coder *coder, int time, char *message)
{
	struct timespec	ts;
	long long		end_time;

	if (!get_running(coder->sim))
		return (0);
	print_status(coder, message);
	pthread_mutex_lock(&coder->sim->state_mutex);
	if (!coder->sim->running)
		return (pthread_mutex_unlock(&coder->sim->state_mutex));
	end_time = get_time_ms() + time;
	while (coder->sim->running)
	{
		ms_to_timespec(end_time, &ts);
		if (pthread_cond_timedwait(&coder->sim->state_cond,
				&coder->sim->state_mutex, &ts) == ETIMEDOUT)
			break ;
	}
	if (!coder->sim->running)
		return (pthread_mutex_unlock(&coder->sim->state_mutex));
	pthread_mutex_unlock(&coder->sim->state_mutex);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_sim		*sim;
	long long	now;

	sim = coder->sim;
	now = get_time_ms();
	pthread_mutex_lock(&sim->dongle_mutex);
	sim->dongles[coder->left_dongle].available = 0;
	sim->dongles[coder->left_dongle].available_at = now + sim->dongle_cooldown;
	sim->dongles[coder->right_dongle].available = 0;
	sim->dongles[coder->right_dongle].available_at = now + sim->dongle_cooldown;
	pthread_cond_broadcast(&sim->dongle_cond);
	pthread_mutex_unlock(&sim->dongle_mutex);
}

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->sim->state_mutex);
	print_status(coder, "is compiling");
	usleep(coder->sim->time_to_compile * 1000);
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->compile_count++;
	if (all_coders_finished(coder->sim))
	{
		coder->sim->running = 0;
		pthread_cond_broadcast(&coder->sim->dongle_cond);
		pthread_cond_broadcast(&coder->sim->state_cond);
	}
	pthread_mutex_unlock(&coder->sim->state_mutex);
}

void	runi(t_sim *sim)
{
	long long		now;
	struct timespec	ts;

	now = get_time_ms() + 10;
	ms_to_timespec(now, &ts);
	pthread_cond_timedwait(&sim->dongle_cond,
		&sim->dongle_mutex, &ts);
}

int	take_dongles(t_coder *coder)
{
	t_sim			*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->dongle_mutex);
	add_request(coder);
	if (coder->left_dongle == coder->right_dongle)
		return (0);
	while (get_running(sim))
	{
		if (is_best_available_request(coder))
			break ;
		runi(sim);
	}
	if (!get_running(sim))
	{
		remove_request(sim, coder);
		pthread_mutex_unlock(&sim->dongle_mutex);
		return (0);
	}
	sim->dongles[coder->left_dongle && coder->right_dongle].available = 0;
	heap_pop(sim, coder);
	pthread_mutex_unlock(&sim->dongle_mutex);
	print_status(coder, "has taken both dongles");
	return (1);
}
