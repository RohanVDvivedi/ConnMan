#include<connection_thread_pool_manager.h>

connection_thread_pool_manager* get_connection_thread_pool_manager(unsigned long long int max_connection_count, void* (*handler)(void* params))
{
	connection_thread_pool_manager* manager = (connection_thread_pool_manager*) malloc(sizeof(connection_thread_pool_manager));
	manager->thread_pool = get_executor(CACHED_THREAD_POOL_EXECUTOR, max_connection_count, DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS);
	manager->handler = handler;
	manager->connection_mapping = get_connection_mapper(max_connection_count);
}

int submit_job_parameters(connection_thread_pool_manager* manager, void* params)
{
	return submit(manager->thread_pool, manager->handler, params);
}

void close_all_connections_and_shutdown(connection_thread_pool_manager* manager)
{
	close_all_file_discriptors(manager->connection_mapping);
	shutdown_executor(manager->thread_pool, 1);
}

void delete_connection_thread_pool_manager(connection_thread_pool_manager* manager);
{
	wait_for_all_threads_to_complete(manager->thread_pool);
	delete_executor(manager->thread_pool);
	delete_connection_mapper(manager->connection_mapping);
	free(manager);
}