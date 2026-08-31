/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 11:16:58 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/19 12:07:17 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	isnum(char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoi(const char *str)
{
	int			i;
	long long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i] - '0');
		if (nbr > 2147483647)
			return (-1);
		i++;
	}
	return ((int)nbr);
}

static int	set_values(t_sim *sim, char **argv)
{
	sim->number_of_coders = ft_atoi(argv[1]);
	sim->time_to_burnout = ft_atoi(argv[2]);
	sim->time_to_compile = ft_atoi(argv[3]);
	sim->time_to_debug = ft_atoi(argv[4]);
	sim->time_to_refactor = ft_atoi(argv[5]);
	sim->number_of_compiles_required = ft_atoi(argv[6]);
	sim->dongle_cooldown = ft_atoi(argv[7]);
	if (sim->number_of_coders <= 0
		|| sim->time_to_burnout <= 0
		|| sim->time_to_compile <= 0
		|| sim->time_to_debug <= 0
		|| sim->time_to_refactor <= 0
		|| sim->number_of_compiles_required <= 0
		|| sim->dongle_cooldown <= 0)
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	if (argc != 9)
	{
		printf("Error: expected 8 arguments\n");
		return (0);
	}
	if (!isnum(argv[1]) || !isnum(argv[2]) || !isnum(argv[3])
		|| !isnum(argv[4]) || !isnum(argv[5]) || !isnum(argv[6])
		|| !isnum(argv[7]))
	{
		printf("Error: invalid numeric argument\n");
		return (0);
	}
	if (!set_values(sim, argv))
	{
		printf("Error: invalid numeric argument\n");
		return (0);
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("Error: invalid scheduler\n");
		return (0);
	}
	sim->scheduler = argv[8];
	return (1);
}
