/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2019 MediaTek Inc.
 */

#ifdef CONFIG_MTK_SCHED_EXTENSION

TRACE_EVENT(sched_select_task_rq,

	TP_PROTO(struct task_struct *tsk,
		int policy, int prev_cpu, int target_cpu,
		int task_util, int boost, bool prefer, int wake_flags),

	TP_ARGS(tsk, policy, prev_cpu, target_cpu, task_util, boost,
		prefer, wake_flags),

	TP_STRUCT__entry(
		__field(pid_t, pid)
		__field(int, policy)
		__field(int, prev_cpu)
		__field(int, target_cpu)
		__field(int, task_util)
		__field(int, boost)
		__field(long, task_mask)
		__field(bool, prefer)
#ifdef CONFIG_MTK_SCHED_CPU_PREFER
		__field(int, cpu_prefer)
#endif
		__field(int, wake_flags)
		),

	TP_fast_assign(
		__entry->pid        = tsk->pid;
		__entry->policy     = policy;
		__entry->prev_cpu   = prev_cpu;
		__entry->target_cpu = target_cpu;
		__entry->task_util	= task_util;
		__entry->boost		= boost;
		__entry->task_mask	= tsk->cpus_allowed.bits[0];
		__entry->prefer		= prefer;
#ifdef CONFIG_MTK_SCHED_CPU_PREFER
		__entry->cpu_prefer = cpu_prefer(tsk);
#endif
		__entry->wake_flags	= wake_flags;
		),

	TP_printk("pid=%4d policy=0x%08x pre-cpu=%d target=%d util=%d boost=%d mask=0x%lx prefer=%d cpu_prefer=%d flags=%d",
		__entry->pid,
		__entry->policy,
		__entry->prev_cpu,
		__entry->target_cpu,
		__entry->task_util,
		__entry->boost,
		__entry->task_mask,
		__entry->prefer,
#ifdef CONFIG_MTK_SCHED_CPU_PREFER
		__entry->cpu_prefer,
#else
		0,
#endif
		__entry->wake_flags)

);

TRACE_EVENT(sched_compute_energy,

	TP_PROTO(int dst_cpu, struct cpumask *pd_mask,
		unsigned long energy, unsigned long max_util, unsigned long sum_util),

	TP_ARGS(dst_cpu, pd_mask, energy, max_util, sum_util),

	TP_STRUCT__entry(
		__field(int, dst_cpu)
		__field(long, cpu_mask )
		__field(unsigned long, energy)
		__field(unsigned long, max_util)
		__field(unsigned long, sum_util)
		),

	TP_fast_assign(
		__entry->dst_cpu    = dst_cpu;
		__entry->cpu_mask   = pd_mask->bits[0];
		__entry->energy     = energy;
		__entry->max_util   = max_util;
		__entry->sum_util   = sum_util;
	),

	TP_printk("dst_cpu=%d mask=0x%lx energy=%lu max_util=%lu sum_util=%lu",
		__entry->dst_cpu,
		__entry->cpu_mask,
		__entry->energy,
		__entry->max_util,
		__entry->sum_util)
);

TRACE_EVENT(sched_energy_util,

	TP_PROTO(int dst_cpu,
		unsigned long max_util, unsigned long sum_util,
		int cpu, unsigned long util_cfs, unsigned long util_cfs_energy,
		unsigned long cpu_util),

	TP_ARGS(dst_cpu, max_util, sum_util, cpu, util_cfs, util_cfs_energy, cpu_util),

	TP_STRUCT__entry(
		__field(int, dst_cpu)
		__field(unsigned long, max_util)
		__field(unsigned long, sum_util)
		__field(int, cpu)
		__field(unsigned long, util_cfs)
		__field(unsigned long, util_cfs_energy)
		__field(unsigned long, cpu_util)
		),

	TP_fast_assign(
		__entry->dst_cpu    = dst_cpu;
		__entry->max_util   = max_util;
		__entry->sum_util   = sum_util;
		__entry->cpu        = cpu;
		__entry->util_cfs   = util_cfs;
		__entry->util_cfs_energy   = util_cfs_energy;
		__entry->cpu_util   = cpu_util;
		),

	TP_printk("dst_cpu=%d max_util=%lu sum_util=%lu cpu=%d util_cfs=%lu util_cfs_energy=%lu cpu_util=%lu",
		__entry->dst_cpu,
		__entry->max_util,
		__entry->sum_util,
		__entry->cpu,
		__entry->util_cfs,
		__entry->util_cfs_energy,
		__entry->cpu_util)
);

TRACE_EVENT(sched_em_cpu_energy,

	TP_PROTO(int opp,
		unsigned long freq,
		unsigned long dyn_pwr,
		unsigned long sum_util, unsigned int opp_cap),

	TP_ARGS(opp, freq, dyn_pwr, sum_util, opp_cap),

	TP_STRUCT__entry(
		__field(int, opp)
		__field(unsigned long, freq)
		__field(unsigned long, dyn_pwr)
		__field(unsigned long, sum_util)
		__field(unsigned int, opp_cap)
	),

	TP_fast_assign(
		__entry->opp        = opp;
		__entry->freq       = freq;
		__entry->dyn_pwr    = dyn_pwr;
		__entry->sum_util   = sum_util;
		__entry->opp_cap    = opp_cap;
	),

	TP_printk("opp=%d freq=%lu dyn_pwr=%lu sum_util=%lu opp_cap=%u",
		__entry->opp,
		__entry->freq,
		__entry->dyn_pwr,
		__entry->sum_util,
		__entry->opp_cap)
);


/*
 * Tracepoint for task migrations.
 */
TRACE_EVENT(sched_migrate,

	TP_PROTO(struct task_struct *tsk, int src, int dest, int force),

	TP_ARGS(tsk, src, dest, force),

	TP_STRUCT__entry(
		__array(char, comm, TASK_COMM_LEN)
		__field(pid_t, pid)
		__field(int,  src)
		__field(int,  dest)
		__field(int,  force)
		),

	TP_fast_assign(
		memcpy(__entry->comm, tsk->comm, TASK_COMM_LEN);
		__entry->pid   = tsk->pid;
		__entry->src  = src;
		__entry->dest  = dest;
		__entry->force = force;
		),

	TP_printk("pid=%d comm=%s src=%d dest=%d force=%d",
		__entry->pid, __entry->comm,
		__entry->src, __entry->dest,
		__entry->force)
);

#ifdef CONFIG_MTK_SCHED_CPU_PREFER
/*
 * Tracepoint for set task cpu prefer
 */
TRACE_EVENT(sched_set_cpuprefer,

	TP_PROTO(struct task_struct *tsk),

	TP_ARGS(tsk),

	TP_STRUCT__entry(
		__array(char,  comm,   TASK_COMM_LEN)
		__field(pid_t, pid)
		__field(int,   cpu_prefer)
	),

	TP_fast_assign(
		memcpy(__entry->comm, tsk->comm, TASK_COMM_LEN);
		__entry->pid            = tsk->pid;
		__entry->cpu_prefer     = tsk->cpu_prefer;
	),

	TP_printk("pid=%d comm=%s cpu_prefer=%d",
		__entry->pid, __entry->comm, __entry->cpu_prefer)
);
#endif

#endif /* CONFIG_MTK_SCHED_EXTENSION */
