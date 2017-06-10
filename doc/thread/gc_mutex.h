pthread_spinlock_t gc_mutex;
pthread_spin_init(&gc_mutex, PTHREAD_PROCESS_PRIVATE);

void gc_lock()
{
}

void gc_unlock()
{
}
