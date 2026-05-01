// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) Linux Test Project, 2026
 * Copyright (c) 2026 Li Wang <li.wang@linux.dev>
 */

#ifndef TST_PATH__
#define TST_PATH__

/* KERNEL */
#define PATH_HOSTNAME			"/proc/sys/kernel/hostname"
#define PATH_OSRELEASE			"/proc/sys/kernel/osrelease"
#define PATH_VERSION			"/proc/sys/kernel/version"
#define PATH_DOMAINNAME			"/proc/sys/kernel/domainname"
#define PATH_PRINTK			"/proc/sys/kernel/printk"
#define PATH_PID_MAX			"/proc/sys/kernel/pid_max"
#define PATH_SHMMAX			"/proc/sys/kernel/shmmax"
#define PATH_SHMMNI			"/proc/sys/kernel/shmmni"
#define PATH_SHMALL			"/proc/sys/kernel/shmall"
#define PATH_SHM_NEXT_ID		"/proc/sys/kernel/shm_next_id"
#define PATH_MSGMNI			"/proc/sys/kernel/msgmni"
#define PATH_MSG_NEXT_ID		"/proc/sys/kernel/msg_next_id"
#define PATH_SEM			"/proc/sys/kernel/sem"
#define PATH_CORE_PATTERN		"/proc/sys/kernel/core_pattern"
#define PATH_CAP_LAST_CAP		"/proc/sys/kernel/cap_last_cap"
#define PATH_NUMA_BALANCING		"/proc/sys/kernel/numa_balancing"
#define PATH_IO_URING_DISABLED		"/proc/sys/kernel/io_uring_disabled"
#define PATH_OVERFLOWUID		"/proc/sys/kernel/overflowuid"
#define PATH_OVERFLOWGID		"/proc/sys/kernel/overflowgid"
#define PATH_PERF_EVENT_PARANOID	"/proc/sys/kernel/perf_event_paranoid"
#define PATH_PERF_EVENT_MLOCK_KB	"/proc/sys/kernel/perf_event_mlock_kb"
#define PATH_PERF_EVENT_MAX_SAMPLE_RATE	"/proc/sys/kernel/perf_event_max_sample_rate"
#define PATH_SCHED_RT_PERIOD_US		"/proc/sys/kernel/sched_rt_period_us"
#define PATH_SCHED_RT_RUNTIME_US	"/proc/sys/kernel/sched_rt_runtime_us"
#define PATH_SCHED_RR_TIMESLICE_MS	"/proc/sys/kernel/sched_rr_timeslice_ms"
#define PATH_UNPRIVILEGED_USERNS_CLONE	"/proc/sys/kernel/unprivileged_userns_clone"

/* USER */
#define PATH_MAX_USER_NAMESPACES	"/proc/sys/user/max_user_namespaces"
#define PATH_MAX_FANOTIFY_GROUPS	"/proc/sys/user/max_fanotify_groups"
#define PATH_MAX_FANOTIFY_MARKS		"/proc/sys/user/max_fanotify_marks"

/* FS */
#define PATH_FS_NR_OPEN			"/proc/sys/fs/nr_open"
#define PATH_FS_NR_AIO_MAX_NR		"/proc/sys/fs/aio-max-nr"
#define PATH_FS_PIPE_MAX_SIZE		"/proc/sys/fs/pipe-max-size"
#define PATH_FS_PIPE_MAX_PAGES		"/proc/sys/fs/pipe-max-pages"
#define PATH_FS_MAX_USER_GROUPS		"/proc/sys/fs/fanotify/max_user_groups"
#define PATH_FS_MAX_USER_MARKS		"/proc/sys/fs/fanotify/max_user_marks"

/* VM */
#define PATH_VM_DROP_CACHES		"/proc/sys/vm/drop_caches"
#define PATH_VM_COMPACT_MEMORY		"/proc/sys/vm/compact_memory"
#define PATH_VM_PANIC_ON_OOM		"/proc/sys/vm/panic_on_oom"
#define PATH_VM_OVERCOMMIT_MEMORY	"/proc/sys/vm/overcommit_memory"
#define PATH_VM_OVERCOMMIT_RATIO	"/proc/sys/vm/overcommit_ratio"
#define PATH_VM_MIN_FREE_KBYTES		"/proc/sys/vm/min_free_kbytes"
#define PATH_VM_VFS_CACHE_PRESSURE	"/proc/sys/vm/vfs_cache_pressure"

/* HUGETLB */
#define PATH_MM_HUGEPAGES		"/sys/kernel/mm/hugepages/"
#define PATH_MM_THP			"/sys/kernel/mm/transparent_hugepage/"
#define PATH_VM_NR_HPAGES		"/proc/sys/vm/nr_hugepages"
#define PATH_VM_OVERCOMMIT_HPAGES	"/proc/sys/vm/nr_overcommit_hugepages"

/* KSM */
#define PATH_MM_KSM		"/sys/kernel/mm/ksm/"
#define MM_KSM_FP(s)		(PATH_MM_KSM s)

/* NETWORK */
#define PATH_NET_IPV4		"/proc/sys/net/ipv4/"
#define NET_IPV4_FP(s)		(PATH_NET_IPV4 s)

/* MEMINFO */
#define MEMINFO_HPAGE_TOTAL	"HugePages_Total:"
#define MEMINFO_HPAGE_FREE	"HugePages_Free:"
#define MEMINFO_HPAGE_RSVD	"HugePages_Rsvd:"
#define MEMINFO_HPAGE_SURP	"HugePages_Surp:"
#define MEMINFO_HPAGE_SIZE	"Hugepagesize:"

#endif /* TST_PATH__ */
