/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:23:54 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 16:37:29 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	can_grant_request(t_sim *sim, t_request *request)
{
	t_dongle	*left;
	t_dongle	*right;
	long long	now;

	left = &sim->dongles[request->left_dongle];
	right = &sim->dongles[request->right_dongle];
	now = get_time_ms();
	if (!left->available && left->available_at > 0
		&& left->available_at <= now)
		left->available = 1;
	if (!right->available && right->available_at > 0
		&& right->available_at <= now)
		right->available = 1;
	if (!left->available || !right->available)
		return (0);
	return (1);
}

int	request_can_run(t_coder *coder)
{
	t_sim		*sim;
	t_heap		*heap;
	int			i;

	sim = coder->sim;
	heap = sim->heap;
	i = 0;
	while (i < heap->size)
	{
		if (heap->requests[i].coder == coder)
			break ;
		i++;
	}
	if (i == heap->size)
		return (0);
	if (!can_grant_request(sim, &heap->requests[i]))
		return (0);
	return (1);
}

int	is_best_available_request(t_coder *coder)
{
	t_heap		*heap;
	t_request	*mine;
	t_request	*request;
	int			i;

	heap = coder->sim->heap;
	mine = find_coder_request(heap, coder);
	if (!mine)
		return (0);
	if (!can_grant_request(coder->sim, mine))
		return (0);
	i = 0;
	while (i < heap->size)
	{
		request = &heap->requests[i];
		if (request->coder != coder
			&& can_grant_request(coder->sim, request)
			&& request_has_priority(coder->sim, request, mine))
			return (0);
		i++;
	}
	return (1);
}

t_request	*heap_peek(t_sim *sim)
{
	if (sim->heap->size == 0)
		return (NULL);
	return (&sim->heap->requests[0]);
}

void	remove_request(t_sim *sim, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < sim->heap->size)
	{
		if (sim->heap->requests[i].coder == coder)
		{
			sim->heap->requests[i]
				= sim->heap->requests[sim->heap->size - 1];
			sim->heap->size--;
			return ;
		}
		i++;
	}
}
