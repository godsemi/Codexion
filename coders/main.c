/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 15:25:58 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/19 13:29:22 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (!parse_args(argc, argv, &sim))
		return (1);
	if (!init_simulation(&sim))
		return (1);
	if (!start_simulation(&sim))
	{
		cleanup_simulation(&sim);
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
