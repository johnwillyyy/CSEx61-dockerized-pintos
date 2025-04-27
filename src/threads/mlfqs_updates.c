#include "fixed-point.h"
#include "thread.h"

#define F 14

void print_load_avg(fixed_t load_avg) {
    int integer_part = load_avg / F;  // Integer part of the load average
    int fractional_part = load_avg % F;  // Remainder (fractional part)
    
    // Convert fractional part to decimal
    // The fractional part is a fixed-point number with 16 fractional bits
    // To get a decimal value, divide it by the scaling factor (F)
    printf("Load Average: %d.%d\n", integer_part, fractional_part * 100 / F);
}

/* Increase recent_cpu by 1. */
void thread_mlfqs_increase_recent_cpu_by_one(void) {
	ASSERT(thread_mlfqs);
	ASSERT(intr_context());

	struct thread *current_thread = thread_current();
	if (current_thread == get_idle_thread())
		return;
	current_thread->recent_cpu = FP_ADD_MIX(current_thread->recent_cpu, 1);
}

void thread_mlfqs_update_recent_cpu_for_thread(struct thread *t) {
    ASSERT(t != NULL);
    ASSERT(t != get_idle_thread());

    // Step 1: Calculate the factor (2 * load_avg) / (2 * load_avg + 1)
    fixed_t term1 = FP_MULT_MIX(load_avg, 2);
    fixed_t term2 = FP_ADD_MIX(FP_MULT_MIX(load_avg, 2), 1);
    fixed_t factor = FP_DIV(term1, term2);

    // Step 2: Calculate recent_cpu = factor * recent_cpu + nice
    t->recent_cpu = FP_ADD_MIX(FP_MULT(factor, t->recent_cpu), t->nice);
}


/* ++1.3 Every per second to refresh load_avg and recent_cpu of all threads. */
void thread_mlfqs_update_load_avg_and_recent_cpu(void) {
	ASSERT(thread_mlfqs);
	ASSERT(intr_context());

	size_t ready_threads = list_size(get_ready_threads());

	if (thread_current() != get_idle_thread())
		ready_threads++;
	// Step 1: Divide 59/60
	fixed_t term1 = FP_DIV_MIX(FP_CONST(59), 60);
	// Step 2: Multiply term1 by load_avg and assign to term1
	term1 = FP_MULT(term1, load_avg);
	// Step 3: Multiply 1/60 by the number of ready threads and assign to term2
	fixed_t term2 = FP_DIV_MIX(FP_CONST(1), 60);
	term2 = FP_MULT_MIX(term2, ready_threads);
	// Step 4: Add term1 and term2
	load_avg = FP_ADD(term1, term2);

	// printf("\nELREADY THREADS = %d\n", ready_threads);
	// msg ("load average=%d.%02d.\n",
	// 	thread_get_load_avg() / 100, thread_get_load_avg() % 100);
		
	struct thread *t;
	struct list_elem *e = list_begin(get_all_threads());
	for (; e != list_end(get_all_threads()); e = list_next(e)) {
		t = list_entry(e, struct thread, allelem);
		if (t != get_idle_thread()) {
			t->recent_cpu = FP_ADD_MIX(FP_MULT(FP_DIV(FP_MULT_MIX(load_avg, 2), FP_ADD_MIX(FP_MULT_MIX(load_avg, 2), 1)), t->recent_cpu), t->nice);
		}
	}
}

/* ++1.3 Update priority. */
void thread_mlfqs_update_priority(struct thread *t) {
    if (t == get_idle_thread())
        return;

    ASSERT(thread_mlfqs);
    ASSERT(t != get_idle_thread());

    t->priority = FP_INT_PART(FP_SUB_MIX(FP_SUB(FP_CONST(PRI_MAX), FP_DIV_MIX(t->recent_cpu, 4)), 2 * t->nice));

    t->priority = t->priority < PRI_MIN ? PRI_MIN : t->priority;
    t->priority = t->priority > PRI_MAX ? PRI_MAX : t->priority;
}


void thread_mlfqs_update_priority_all(void) {
    ASSERT(thread_mlfqs);
    struct thread *t;
    struct list_elem *e = list_begin(get_all_threads());

    fixed_t two_load_avg = FP_MULT_MIX(load_avg, 2);  // Calculate 2 * load_avg once

    for (; e != list_end(get_all_threads()); e = list_next(e)) {
        t = list_entry(e, struct thread, allelem);
        if (t != get_idle_thread()) {
            // Update recent_cpu based on the formula:
            // Update priority for the thread
            thread_mlfqs_update_priority(t);
        }
    }
}
