/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 13:23:36 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 16:28:14 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	add_request(t_coder *coder)
{
	t_request	request;
	t_sim		*sim;

	sim = coder->sim;
	request.coder = coder;
	request.left_dongle = coder->left_dongle;
	request.right_dongle = coder->right_dongle;
	request.deadline = coder->last_compile_start
		+ sim->time_to_burnout;
	request.order = sim->request_order++;
	heap_push(sim, request);
}

int	can_take_dongles(t_coder *coder)
{
	t_sim		*sim;
	t_dongle	*left;
	t_dongle	*right;
	long long	now;

	sim = coder->sim;
	left = &sim->dongles[coder->left_dongle];
	right = &sim->dongles[coder->right_dongle];
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

int	is_next_request(t_coder *coder)
{
	t_request	*request;

	request = heap_peek(coder->sim);
	if (!request)
		return (0);
	if (request->coder == coder)
		return (1);
	return (0);
}

t_request	*find_coder_request(t_heap *heap, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->requests[i].coder == coder)
			return (&heap->requests[i]);
		i++;
	}
	return (NULL);
}
