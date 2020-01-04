#ifndef CONNECTION_THREAD_POOL_MANAGER_H
#define CONNECTION_THREAD_POOL_MANAGER_H

#include<stdio.h>
#include<stdlib.h>

#include<executor.h>

#include<connection_mapping.h>

typedef struct connection_thread_pool_manager connection_thread_pool_manager;
struct connection_thread_pool_manager
{
	// this is the executor service the connection thread pool will be built on
	executor* thread_pool;

	// the handler that will be used by the thread_pool,
	// this way you will only have to submit the parameters
	void* (*handler)(void* params);

	// the threads of thread_pool, have to remember which connection they were working on
	// this hashmap is shared among the threads to access, their own file discriptors
	connection_mapper* connection_mapping;
};

#define DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS 3000 * 1000		// 3 seconds
// in the cached thread_pool if the thread does not receive a job in this time period, it will terminate itself
// this is not applicable for fixed thread pools

// this function will create a CACHED_THREAD_POOL_EXECUTOR executor, each of whose threads, are responsible for handling the connections individually
connection_thread_pool_manager* get_cached_connection_thread_pool_manager(unsigned long long int max_connection_count, void* (*handler)(void* params));

// this function will create a FIXED_THREAD_COUNT_EXECUTOR executor, each of whose threads, are responsible for handling the connections individually
connection_thread_pool_manager* get_fixed_connection_thread_pool_manager(unsigned long long int connection_count, void* (*handler)(void* params));

// you may not call this function, after calling close_all_connections_and_shutdown
// returns 1 if the job was submitted, else 0
int submit_job_parameters(connection_thread_pool_manager* manager, void* params);

// you may not call this function, after calling close_all_connections_and_shutdown
// with this function a job is submitted just like the submit_jon_parameters function, but with a promise that the result will be returned to you
// returns non NULL job pointer, to wait if the job was submitted, else Null
job* submit_job_with_promise(connection_thread_pool_manager* manager, void* params);
// this is the function yopu call, if you submitted a job with a promise
void* get_promised_result(job* job_p);

// you must call this function before calling delete_connection_thread_pool_manager
// note: the shutdown of the thread_pool of the connection_thread_pool_manager is going to be immediate
// i.e. all the left over transactions will be deleted
void close_all_connections_and_wait_for_shutdown(connection_thread_pool_manager* manager);

// this function will only delete the resources occupied by the manager, 
// you must call void close_all_connections_and_wait_for_shutdown(connection_thread_pool_manager* manager), before deletion
void delete_connection_thread_pool_manager(connection_thread_pool_manager* manager);

// these are the methods of the connection thread pool manager,
// that will do the task for mapping of threads, when called by corresponding threads

// returns the file discriptor, that was mapped with the given thread
int get_file_discriptor_for_current_thread(connection_thread_pool_manager* manager);

// insert mapping between the current thread and the given file_discriptor fd
void insert_mapping_with_current_thread(connection_thread_pool_manager* manager, int fd);

// remove the mapping to the file_discriptor, of the current thread, that is in use
// returns the number of mappings removed from the connection_mapper
int remove_mapping_for_current_thread(connection_thread_pool_manager* manager);

#endif