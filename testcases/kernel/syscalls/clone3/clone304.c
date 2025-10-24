// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2025 Red Hat Inc. All Rights Reserved.
 * Author: Chunfu Wen <chwen@redhat.com>
 */

/*\
 * Test that in a thread started by clone3() that runs in the same address
 * space (CLONE_VM) but with a different TLS (CLONE_SETTLS) writtes to a
 * thread local variables are not propagated back from the cloned thread.
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/wait.h>

#if defined(__i386__)
#include <asm/ldt.h>
#endif

#include "tst_test.h"
#include "lapi/sched.h"
#include "lapi/pidfd.h"
#include "lapi/tls.h"

#define TLS_EXP 100

#define CHILD_STACK_SIZE (1024*1024)

// Global pointers for TLS management
void *tls_ptr;
struct user_desc *tls_desc;

static int pidfd, child_tid, parent_tid;
static struct clone_args *args;
static void *child_stack;

/* TLS variable to validate in child */
static __thread int tls_var;

static int test_flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SETTLS;

static int check_tls_in_child(void)
{
	tls_var = TLS_EXP + 1;
	tst_res(TINFO, "Child (PID: %d, TID: %d): TLS value set to: %d", getpid(),  gettid(), tls_var);

	free_tls();
	tst_syscall(__NR_exit, 0);
	return 0;
}

static void run(void)
{
	pid_t pid;

	tls_var = TLS_EXP;

	args->flags = test_flags;
	args->pidfd = (uint64_t)(&pidfd);
	args->child_tid = (uint64_t)(&child_tid);
	args->parent_tid = (uint64_t)(&parent_tid);
	args->stack = (uint64_t)(child_stack);
	args->stack_size = CHILD_STACK_SIZE;
	args->tls = (uint64_t)tls_ptr;

	TEST(pid = clone3(args, sizeof(*args)));

	if (TST_RET == -1)
		tst_brk(TBROK | TTERRNO, "clone() failed");

	if (!pid)
		check_tls_in_child();

	waitpid(pid, NULL, 0);

	if (tls_var == TLS_EXP) {
		tst_res(TPASS,
			"Parent (PID: %d, TID: %d): TLS value correct: %d",
			getpid(), gettid(), tls_var);
	} else {
		tst_res(TFAIL,
			"Parent (PID: %d, TID: %d): TLS value mismatch: got %d, expected %d",
			getpid(), gettid(), tls_var, TLS_EXP);
	}
}

static void setup(void)
{
	clone3_supported_by_kernel();
	child_stack = SAFE_MALLOC(CHILD_STACK_SIZE);
	init_tls();
}

static void cleanup(void)
{
	free(child_stack);
}

static struct tst_test test = {
	.setup = setup,
	.cleanup = cleanup,
	.needs_checkpoints = 1,
	.test_all = run,
	.bufs = (struct tst_buffers []) {
		{&args, .size = sizeof(*args)},
		{},
	}
};
