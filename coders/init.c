/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 12:42:16 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 15:56:40 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].left_dongle = i;
		sim->coders[i].right_dongle = (i + 1) % sim->number_of_coders;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].sim = sim;
		i++;
	}
	return (1);
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		sim->coders = NULL;
		return (0);
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].available = 1;
		sim->dongles[i].available_at = 0;
		i++;
	}
	return (1);
}

int	destroy(t_sim *sim, int i)
{
	if (i == 3)
	{
		freer(sim);
	}
	if (i == 0)
	{
		pthread_cond_destroy(&sim->dongle_cond);
		pthread_mutex_destroy(&sim->dongle_mutex);
	}
	else if (i == 1)
	{
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_cond_destroy(&sim->dongle_cond);
		pthread_mutex_destroy(&sim->dongle_mutex);
	}
	else if (i == 2 || i == 3)
	{
		pthread_cond_destroy(&sim->state_cond);
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_cond_destroy(&sim->dongle_cond);
		pthread_mutex_destroy(&sim->dongle_mutex);
	}
	return (0);
}

int	init_simulation(t_sim *sim)
{
	sim->running = 1;
	sim->start_time = get_time_ms();
	sim->request_order = 0;
	sim->coders = NULL;
	sim->dongles = NULL;
	if (pthread_mutex_init(&sim->dongle_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->dongle_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->dongle_mutex);
		return (0);
	}
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (destroy(sim, 0));
	if (pthread_cond_init(&sim->state_cond, NULL) != 0)
		return (destroy(sim, 1));
	if (!init_coders(sim))
		return (destroy(sim, 2));
	if (!init_dongles(sim))
		return (destroy(sim, 2));
	if (!init_heap(sim))
		return (destroy(sim, 3));
	return (1);
}

int	init_heap(t_sim *sim)
{
	sim->heap = malloc(sizeof(t_heap));
	if (!sim->heap)
		return (0);
	sim->heap->capacity = sim->number_of_coders;
	sim->heap->size = 0;
	sim->heap->requests = malloc(sizeof(t_request)
			* sim->heap->capacity);
	if (!sim->heap->requests)
	{
		free(sim->heap);
		sim->heap = NULL;
		return (0);
	}
	return (1);
}
