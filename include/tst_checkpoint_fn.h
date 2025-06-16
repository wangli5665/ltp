/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (c) 2015-2016 Cyril Hrubis <chrubis@suse.cz>
 */

#ifndef TST_CHECKPOINT_FN__
#define TST_CHECKPOINT_FN__

/*
 * Checkpoint initialization.
 *
 * This function sets up the shared memory region used for process
 * synchronization via futexes. It must be called before any checkpoint
 * operations such as tst_checkpoint_wait() or tst_checkpoint_wake().
 */
void tst_checkpoint_init(const char *file, const int lineno,
			 void (*cleanup_fn)(void));

/*
 * Checkpoint reinitialization.
 *
 * This function re-attaches to an existing shared memory checkpoint region
 * pointed to by the LTP_IPC_PATH environment variable. It is typically used
 * in child processes (e.g., shell scripts) to synchronize with the main test.
 *
 * The function verifies the magic header in the shared memory file and maps
 * the futex array into memory. It must be called before using checkpoint
 * operations in a process that did not perform the original initialization.
 */
void tst_checkpoint_reinit(const char *file, const int lineno,
			   void (*cleanup_fn)(void));

/*
 * Waits for wakeup.
 *
 * @id: Checkpoint id, positive number
 * @msec_timeout: Timeout in milliseconds, 0 == no timeout
 */
int tst_checkpoint_wait(unsigned int id, unsigned int msec_timeout);

/*
 * Wakes up sleeping process(es)/thread(s).
 *
 * @id: Checkpoint id, positive number
 * @nr_wake: Number of processes/threads to wake up
 * @msec_timeout: Timeout in milliseconds, 0 == no timeout
 */
int tst_checkpoint_wake(unsigned int id, unsigned int nr_wake,
                        unsigned int msec_timeout);

void tst_safe_checkpoint_wait(const char *file, const int lineno,
                              void (*cleanup_fn)(void), unsigned int id,
			      unsigned int msec_timeout);

void tst_safe_checkpoint_wake(const char *file, const int lineno,
                              void (*cleanup_fn)(void), unsigned int id,
                              unsigned int nr_wake);

#endif /* TST_CHECKPOINT_FN__ */
