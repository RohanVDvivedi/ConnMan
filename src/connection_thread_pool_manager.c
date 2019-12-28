#include<connection_thread_pool_manager.h>

connection_thread_pool_manager* get_cached_connection_thread_pool_manager(unsigned long long int max_connection_count, void* (*handler)(void* params))
{
	connection_thread_pool_manager* manager = (connection_thread_pool_manager*) malloc(sizeof(connection_thread_pool_manager));
	manager->thread_pool = get_executor(CACHED_THREAD_POOL_EXECUTOR, max_connection_count, DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS);
	manager->handler = handler;
	manager->connection_mapping = get_connection_mapper(max_connection_count);
	return manager;
}

connection_thread_pool_manager* get_fixed_connection_thread_pool_manager(unsigned long long int connection_count, void* (*handler)(void* params))
{
	connection_thread_pool_manager* manager = (connection_thread_pool_manager*) malloc(sizeof(connection_thread_pool_manager));
	manager->thread_pool = get_executor(FIXED_THREAD_COUNT_EXECUTOR, connection_count, -1);
	manager->handler = handler;
	manager->connection_mapping = get_connection_mapper(connection_count);
	return manager;
}

int submit_job_parameters(connection_thread_pool_manager* manager, void* params)
{
	return submit(manager->thread_pool, manager->handler, params);
}

void close_all_connections_and_wait_for_shutdown(connection_thread_pool_manager* manager, int immediately)
{
	close_all_file_discriptors(manager->connection_mapping);
	shutdown_executor(manager->thread_pool, immediately);
	wait_for_all_threads_to_complete(manager->thread_pool);
}

void delete_connection_thread_pool_manager(connection_thread_pool_manager* manager)
{
	delete_executor(manager->thread_pool);
	delete_connection_mapper(manager->connection_mapping);
	free(manager);
}

int get_file_discriptor_for_current_thread(connection_thread_pool_manager* manager)
{
	return get_mapping(manager->connection_mapping, pthread_self());
}

void insert_mapping_with_current_thread(connection_thread_pool_manager* manager, int fd)
{
	insert_mapping(manager->connection_mapping, pthread_self(), fd);
}

int remove_mapping_for_current_thread(connection_thread_pool_manager* manager)
{
	return remove_mapping(manager->connection_mapping, pthread_self());
}
