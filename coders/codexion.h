/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopaz-su <jopaz-su@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 15:24:51 by jopaz-su          #+#    #+#             */
/*   Updated: 2026/08/31 16:39:30 by jopaz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	int			left_dongle;
	int			right_dongle;
	long long	last_compile_start;
	pthread_t	thread;
	t_sim		*sim;
}	t_coder;

typedef struct s_dongle
{
	int			available;
	long long	available_at;
}	t_dongle;

typedef struct s_request
{
	t_coder		*coder;
	int			left_dongle;
	int			right_dongle;
	long		deadline;
	long		order;
}	t_request;

typedef struct s_heap
{
	t_request	*requests;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_sim
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				running;
	long			request_order;
	char			*scheduler;
	long long		start_time;

	t_coder			*coders;
	t_dongle		*dongles;

	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
	pthread_mutex_t	state_mutex;
	pthread_cond_t	state_cond;
	pthread_t		monitor_thread;

	t_heap			*heap;
}	t_sim;

int				parse_args(int argc, char **argv, t_sim *sim);
int				init_simulation(t_sim *sim);
int				init_coders(t_sim *sim);
int				init_dongles(t_sim *sim);
int				start_simulation(t_sim *sim);
void			cleanup_simulation(t_sim *sim);
void			*coder_routine(void *arg);
int				take_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);
int				coder_wait(t_coder *coder, int time, char *message);
long long		get_time_ms(void);
void			print_status(t_coder *coder, char *message);
void			ms_to_timespec(long long ms, struct timespec *ts);
int				all_coders_finished(t_sim *sim);
int				get_running(t_sim *sim);
void			set_running(t_sim *sim, int value);
void			*monitor_routine(void *arg);
int				init_heap(t_sim *sim);
int				request_has_priority(t_sim *sim, t_request *a, t_request *b);
void			swap_requests(t_request *a, t_request *b);
void			heap_push(t_sim *sim, t_request req);
t_request		*heap_peek(t_sim *sim);
void			heap_pop(t_sim *sim, t_coder *coder);
void			add_request(t_coder *coder);
int				can_take_dongles(t_coder *coder);
int				is_next_request(t_coder *coder);
void			remove_request(t_sim *sim, t_coder *coder);
int				can_grant_request(t_sim *sim, t_request *request);
int				request_can_run(t_coder *coder);
int				is_best_available_request(t_coder *coder);
void			compile(t_coder *coder);
void			freer(t_sim *sim);
t_request		*find_coder_request(t_heap *heap, t_coder *coder);

#endif
