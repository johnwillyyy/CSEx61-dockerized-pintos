#include "thread.h"

/*
 * Update recent_cpu for each thread based on the load average and the thread's nice value
 * recent_cpu = (2*load_avg)/(2*load_avg + 1) * recent_cpu + nice
 */
void update_threads_recent_cpu(){
    if (thread_current() == get_idle_thread())
		return;
	ASSERT(thread_mlfqs);
    
    struct list_elem *e;

    // Iterate over all threads in the system
    for (e = list_begin(get_all_threads()); e != list_end(get_all_threads()); e = list_next(e)) {
        struct thread *thread = list_entry(e, struct thread, elem);
        if (thread == get_idle_thread())
            continue;

        // term1 = 2*load_avg
        fixed_t term1 = real_multiply_by_int(&load_avg, 2);

        // term2 = 2*load_avg + 1
        fixed_t term2 = real_add_to_int(&term1, 1);

        // Combine term1 and term2 for the final calculation of recent_cpu
        term1 = real_divide(&term1, &term2);
        
        // Multiply the result by the current thread's recent_cpu
        term1 = real_multiply(&term1, &thread->recent_cpu);

        // Update the thread's recent_cpu value
        thread->recent_cpu.value = real_add_to_int(&term1, thread->nice).value;
    }    
}


/*
 * Updates the system load average using the formula:
 * load_avg = (59/60) * load_avg + (1/60) * ready_threads
 * Where:
 * - load_avg is a global fixed-point value representing system load.
 * - ready_threads is the number of threads ready to run (excluding the idle thread).
 */
void update_load_avg() {
    fixed_t numerator;
    
    // Calculate coefficient: 59/60
    real_init(&numerator, 59);
    fixed_t denominator;
    real_init(&denominator, 60);
    fixed_t coefficient = real_divide(&numerator, &denominator);

    // Multiply current load_avg by 59/60
    load_avg.value = real_multiply(&coefficient, &load_avg).value;

    // Reuse numerator and denominator to calculate 1/60
    real_init(&numerator, 1);
    real_init(&denominator, 60);
    coefficient = real_divide(&numerator, &denominator);

    int running = 1;
    if(thread_current() == get_idle_thread())
        running = 0;

    // Multiply (1/60) by the number of ready threads
    coefficient = real_multiply_by_int(&coefficient, list_size(get_ready_threads()) + running);

    // Add the two terms together: load_avg = (59/60)*load_avg + (1/60)*ready_threads
    load_avg.value += coefficient.value;
}


/* 
 * Updates the priority of one thread based on the formula:
 * priority = PRI_MAX - (recent_cpu / 4) - (nice * 2)
 * Priorities are clamped between PRI_MIN and PRI_MAX.
 */
void update_thread_priority(struct thread *thread) {
    if (thread == get_idle_thread())
        return;
	ASSERT(thread_mlfqs);

    // Start with the maximum priority value
    fixed_t term1;
    real_init(&term1, PRI_MAX);

    // term2 = recent_cpu / 4
    fixed_t term2 = real_divide_by_int(&thread->recent_cpu, 4);

    // term3 = nice * 2 (converted to fixed-point)
    fixed_t term3;
    real_init(&term3, thread->nice * 2);

    // term1 = PRI_MAX - (recent_cpu / 4) - (nice * 2)
    term1 = real_subtract(&term1, &term2);
    term1 = real_subtract(&term1, &term3);

    // Convert the result to integer priority (truncated)
    int priority = real_to_int(&term1);

    // Clamp the result within allowed priority bounds
    if (priority > PRI_MAX) priority = PRI_MAX;
    else if (priority < PRI_MIN) priority = PRI_MIN;

    // Assign the new priority
    thread->priority = priority;
    // printf("\nThread Name: %d . Thread priority: %d \n", thread->tid, thread->priority);
}

//Updates the priority of all threads
void update_threads_priority() {
    // if (strcmp(thread_current()->name, "idle"))
	// 	return;
	ASSERT(thread_mlfqs);
    struct list_elem *e;

    // Iterate over all threads in the system
    for (e = list_begin(get_all_threads()); e != list_end(get_all_threads()); e = list_next(e)) {
        struct thread *thread = list_entry(e, struct thread, elem);
        update_thread_priority(thread);
    }
    list_sort(get_ready_threads(), mlfqs_comparator, NULL);
}