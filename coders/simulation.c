/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 16:56:16 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/19 13:30:00 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_sim *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (0);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
	return (1);
}

void	cleanup_simulation(t_sim *sim)
{
	if (sim->heap)
	{
		free(sim->heap->requests);
		free(sim->heap);
	}
	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
		free(sim->dongles);
	pthread_cond_destroy(&sim->state_cond);
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_cond_destroy(&sim->dongle_cond);
	pthread_mutex_destroy(&sim->dongle_mutex);
}
