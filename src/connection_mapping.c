#include<connection_mapping.h>

// utility functions for the hashmap used in connection_mapping ---->>>>

unsigned long long int thread_id_hash(const pthread_t* thread_id)
{
	return ((unsigned long long int)(*thread_id));
}

int compare_thread_ids(const pthread_t* a, const pthread_t* b)
{
	return ((*a) - (*b));
}

// utility functions for the hashmap used in connection_mapping <<<<----

connection_mapper* get_connection_mapper(unsigned long long int expected_connections)
{
	connection_mapper* conn_map_p = (connection_mapper*)malloc(sizeof(connection_mapper));
	initialize_hashmap(&(conn_map_p->thread_id_to_file_discriptor), ((expected_connections/3) + 5), (unsigned long long int (*)(const void*))(thread_id_hash), (int (*)(const void*, const void*))(compare_thread_ids), ELEMENTS_AS_RED_BLACK_BST);
	initialize_rwlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	return conn_map_p;
}

int get_mapping(connection_mapper* conn_map_p, pthread_t tid)
{
	read_lock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	int* fd_p = (int*)find_value_from_hash(&(conn_map_p->thread_id_to_file_discriptor), &tid);
	int fd = ((fd_p == NULL) ? (-1) : (*fd_p));
	read_unlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	return fd;
}

void insert_mapping(connection_mapper* conn_map_p, pthread_t tid, int fd)
{
	write_lock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	int* fd_p = (int*) malloc(sizeof(int));
	(*fd_p) = fd;
	pthread_t* tid_p = (pthread_t*) malloc(sizeof(pthread_t));
	(*tid_p) = tid;
	insert_entry_in_hash(&(conn_map_p->thread_id_to_file_discriptor), tid_p, fd_p);
	write_unlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
}

int remove_mapping(connection_mapper* conn_map_p, pthread_t tid)
{
	write_lock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	const void* rkey = NULL;
	const void* rvalue = NULL;
	int entry_deleted = delete_entry_from_hash(&(conn_map_p->thread_id_to_file_discriptor), &tid, &rkey, &rvalue);
	if(entry_deleted)
	{
		if(rkey != NULL)
		{free((pthread_t*)rkey);}
		if(rvalue != NULL)
		{free((int*)rvalue);}
	}
	write_unlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	return entry_deleted;
}

void delete_entry_operation(pthread_t* tid_p, int* fd_p, const void* ap)
{
	free(tid_p);
	free(fd_p);
}

void delete_all_mappings(connection_mapper* conn_map_p)
{
	write_lock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	for_each_entry_in_hash(&(conn_map_p->thread_id_to_file_discriptor), (void (*)(const void*, const void*, const void*))(delete_entry_operation), NULL);
	write_unlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
}

void close_entry_operation(pthread_t* tid_p, int* fd_p, const void* ap)
{
	close(*fd_p);
}

void close_all_file_discriptors(connection_mapper* conn_map_p)
{
	read_lock(&(conn_map_p->thread_id_to_file_discriptor_lock));
	for_each_entry_in_hash(&(conn_map_p->thread_id_to_file_discriptor), (void (*)(const void*, const void*, const void*))(close_entry_operation), NULL);
	read_unlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
}

void delete_connection_mapper(connection_mapper* conn_map_p)
{
	// delete all the elements of the thread_id_to_file_discriptor hashmap
	delete_all_mappings(conn_map_p);
	deinitialize_hashmap(&(conn_map_p->thread_id_to_file_discriptor));
	deinitialize_rwlock(&(conn_map_p->thread_id_to_file_discriptor_lock));
}
