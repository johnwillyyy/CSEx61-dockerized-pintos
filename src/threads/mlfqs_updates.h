/* update recent_cpu for each threads */
void update_threads_recent_cpu();

/* update load_avg of the system */
void update_load_avg();

/* update priority for one threads */
void update_thread_priority(struct thread *thread);

/* update priorites for all threads */
void update_threads_priority();