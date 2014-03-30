/*
 * A simple non-preemptive interrupt-driven scheduler.
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <inttypes.h>

/**
 * A simple representation of a "task," containing a period,
 * a function to run, a display name, and a released flag (to
 * be set by the scheduler).
 */
typedef struct task_t {
	uint16_t period;
	void (*interrupt_function)(void);
	char released;
	char* name;
} Task;

/**
 * Start the scheduler. This must be called on startup.
 */
void initialize_scheduler();

/**
 * Called by a cyclic executor to release tasks that have
 * been marked ready to run by the scheduler
 */
void release_ready_tasks();

/**
 * Call this if you change the period of a task, as the scheduler
 * may need to re-calculate a new hyperperiod value based on the
 * new task period.
 */
void recalculate_hyperperiod();

/**
 * Register a task to be scheduled.
 */
void register_task(volatile Task* task);

#endif /* SCHEDULER_H_ */
