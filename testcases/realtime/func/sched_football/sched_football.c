// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright © International Business Machines Corp., 2007, 2008
 * Copyright (c) 2024 Petr Vorel <pvorel@suse.cz>
 * Author: John Stultz <jstultz@google.com>
 */

/*\
 * Scheduler test that uses a football analogy.
 *
 * The premise is that we want to make sure that lower priority threads
 * don't run while we have runnable higher priority threads.
 * The offense is trying to increment the balls position, while the
 * defense is trying to block that from happening.
 * And the ref (highest priority thread) will blow the wistle if the
 * ball moves. Finally, we have crazy fans (higer prority) that try to
 * distract the defense by occasionally running onto the field.
 *
 * [Algorithm]
 *
 * - Create NR_CPU offense threads (lower priority).
 * - Create NR_CPU defense threads (mid priority).
 * - Create 2*NR_CPU fan threads (high priority).
 * - Create a referee thread (highest priority).
 * - Once everyone is on the field, the offense thread spins incrementing
 *   the value of 'the_ball'. The defense thread tries to block 'the_ball'
 *   by never letting the offense players get the CPU (it just spins).
 *   The crazy fans sleep a bit, then jump the rail and run across the
 *   field, disrupting the players on the field.
 * - The refree threads wakes up regularly to check if the game is over :).
 * - If the value of 'the_ball' is > 0, the test is considered to have failed.
 */

#include <sys/prctl.h>
#include <tst_atomic.h>
#include <tst_timer.h>
#include "librttest.h"
#include "tst_test.h"

#define DEF_GAME_LENGTH 5
#define SPIN_TIME_NS 200000000ULL
#define SLEEP_TIME_NS 50000000ULL

static tst_atomic_t the_ball;
static int players_per_team = 0;
static int game_length = DEF_GAME_LENGTH;
static tst_atomic_t players_ready;

static char *str_game_length;
static char *str_players_per_team;

/* These are fans running across the field. They're trying to interrupt/distract everyone */
void *thread_fan(void *arg LTP_ATTRIBUTE_UNUSED)
{
	prctl(PR_SET_NAME, "crazy_fan", 0, 0, 0);
	tst_atomic_add_return(1, &players_ready);
	/*occasionally wake up and run across the field */
	while (1) {
		struct timespec start, stop;
		nsec_t nsec;

		start.tv_sec = 0;
		start.tv_nsec = SLEEP_TIME_NS;
		clock_nanosleep(CLOCK_MONOTONIC, 0, &start, NULL);
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		nsec = tst_timespec_diff_ns(stop, start);
		while (nsec < SPIN_TIME_NS) {
			clock_gettime(CLOCK_MONOTONIC, &stop);
			nsec = tst_timespec_diff_ns(stop, start);
		}
	}

	return NULL;
}

/* This is the defensive team. They're trying to block the offense */
void *thread_defense(void *arg LTP_ATTRIBUTE_UNUSED)
{
	prctl(PR_SET_NAME, "defense", 0, 0, 0);
	tst_atomic_add_return(1, &players_ready);
	/*keep the ball from being moved */
	while (1) {
	}

	return NULL;
}

/* This is the offensive team. They're trying to move the ball */
void *thread_offense(void *arg LTP_ATTRIBUTE_UNUSED)
{
	prctl(PR_SET_NAME, "offense", 0, 0, 0);
	tst_atomic_add_return(1, &players_ready);
	while (1) {
		tst_atomic_add_return(1, &the_ball); /* move the ball ahead one yard */
	}

	return NULL;
}

void referee(int game_length)
{
	struct timeval start, now;
	int final_ball;

	tst_res(TINFO, "Starting referee thread");

	prctl(PR_SET_NAME, "referee", 0, 0, 0);
	tst_res(TINFO, "Starting the game (%d sec)", game_length);

	/* open trace marker early to avoid latency with the first message */
	trace_marker_prep();
	gettimeofday(&start, NULL);
	now = start;

	/* Start the game! */
	tst_atomic_store(0, &the_ball);
	atrace_marker_write("sched_football", "Game_started!");

	/* Watch the game */
	while ((now.tv_sec - start.tv_sec) < game_length) {
		sleep(1);
		gettimeofday(&now, NULL);
	}
	atrace_marker_write("sched_football", "Game_Over!");
	final_ball = tst_atomic_load(&the_ball);

	/* Blow the whistle */
	tst_res(TINFO, "Final ball position: %d", final_ball);

	TST_EXP_EXPR(final_ball == 0);
}

static void do_test(void)
{
	struct sched_param param;
	int priority;
	int i;

	if (players_per_team == 0)
		players_per_team = get_numcpus();

	tst_atomic_store(0, &players_ready);

	tst_res(TINFO, "players_per_team: %d game_length: %d",
	       players_per_team, game_length);

	/* We're the ref, so set our priority right */
	param.sched_priority = sched_get_priority_min(SCHED_FIFO) + 80;
	sched_setscheduler(0, SCHED_FIFO, &param);

	/*
	 * Start the offense
	 * They are lower priority than defense, so they must be started first.
	 */
	priority = 15;
	tst_res(TINFO, "Starting %d offense threads at priority %d",
	       players_per_team, priority);
	for (i = 0; i < players_per_team; i++)
		create_fifo_thread(thread_offense, NULL, priority);

	/* Wait for the offense threads to start */
	while (tst_atomic_load(&players_ready) < players_per_team)
		usleep(100);

	/* Start the defense */
	priority = 30;
	tst_res(TINFO, "Starting %d defense threads at priority %d",
	       players_per_team, priority);
	for (i = 0; i < players_per_team; i++)
		create_fifo_thread(thread_defense, NULL, priority);

	/* Wait for the defense threads to start */
	while (tst_atomic_load(&players_ready) < players_per_team * 2)
		usleep(100);

	/* Start the crazy fans*/
	priority = 50;
	tst_res(TINFO, "Starting %d fan threads at priority %d",
	       players_per_team, priority);
	for (i = 0; i < players_per_team*2; i++)
		create_fifo_thread(thread_fan, NULL, priority);

	/* Wait for the crazy fan threads to start */
	while (tst_atomic_load(&players_ready) < players_per_team * 4)
		usleep(100);

	/* let things get into steady state */
	sleep(2);
	/* Ok, everyone is on the field, bring out the ref */

	referee(game_length);
}

static void do_setup(void)
{
	if (tst_parse_int(str_game_length, &game_length, 1, INT_MAX))
		tst_brk(TBROK, "Invalid game length '%s'", str_game_length);

	if (tst_parse_int(str_players_per_team, &players_per_team, 1, INT_MAX))
		tst_brk(TBROK, "Invalid number of players '%s'", str_players_per_team);
}

static struct tst_test test = {
	.test_all = do_test,
	.setup = do_setup,
	.needs_root = 1,
	.options = (struct tst_option[]) {
		{"l:", &str_game_length, "Game length in sec (default: "
			TST_TO_STR(DEF_GAME_LENGTH) " sec)"},
		{"n:", &str_players_per_team,
			"Number of players (default: number of CPU)"},
		{}
	},
};
