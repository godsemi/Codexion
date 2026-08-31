/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:53:55 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 16:22:57 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_has_priority(t_sim *sim, t_request *a, t_request *b)
{
	if (strcmp(sim->scheduler, "fifo") == 0)
		return (a->order < b->order);
	return (a->deadline < b->deadline);
}

void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_push(t_sim *sim, t_request req)
{
	int			i;
	t_request	*heap;

	heap = sim->heap->requests;
	i = sim->heap->size;
	heap[i] = req;
	sim->heap->size++;
	while (i > 0)
	{
		if (!request_has_priority(sim,
				&heap[i], &heap[(i - 1) / 2]))
			break ;
		swap_requests(&heap[i],
			&heap[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

static int	find_coder_index(t_sim *sim, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < sim->heap->size)
	{
		if (sim->heap->requests[i].coder == coder)
			return (i);
		i++;
	}
	return (-1);
}

void	heap_pop(t_sim *sim, t_coder *coder)
{
	t_request	*heap;
	int			i;
	int			child;

	heap = sim->heap->requests;
	i = find_coder_index(sim, coder);
	if (i == -1)
		return ;
	sim->heap->size--;
	heap[i] = heap[sim->heap->size];
	while (1)
	{
		child = i * 2 + 1;
		if (child >= sim->heap->size)
			break ;
		if (child + 1 < sim->heap->size
			&& request_has_priority(sim,
				&heap[child + 1], &heap[child]))
			child++;
		if (!request_has_priority(sim, &heap[child], &heap[i]))
			break ;
		swap_requests(&heap[i], &heap[child]);
		i = child;
	}
}
