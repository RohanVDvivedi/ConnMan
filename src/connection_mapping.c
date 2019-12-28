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

connection_mapper* get_connection_mapper()
{
	conn_grp_p->thread_id_to_file_discriptor = get_hashmap(10, (unsigned long long int (*)(const void*))(thread_id_hash), (int (*)(const void*, const void*))(compare_thread_ids), ELEMENTS_AS_RED_BLACK_BST);
	conn_grp_p->thread_id_to_file_discriptor_lock = get_rwlock();
}

int get(connection_mapper* conn_map_p)
{
	return get_mapping(conn_map_p, pthread_self());
}

int get_mapping(connection_mapper* conn_map_p, pthread_t tid)
{
	read_lock(conn_map_p->thread_id_to_file_discriptor_lock);

	int fd = *(int*)find_value_from_hash(conn_grp_p->thread_id_to_file_discriptor, &tid);

	read_unlock(conn_map_p->thread_id_to_file_discriptor_lock);

	return fd;
}

void insert(connection_mapper* conn_map_p, int fd)
{
	insert_mapping(conn_map_p, pthread_self(), fd);
}

void insert_mapping(connection_mapper* conn_map_p, pthread_t tid, int fd)
{
	write_lock(conn_map_p->thread_id_to_file_discriptor_lock);

	int* fd_p = (int*) malloc(sizeof(int));
	(*fd_p) = fd;

	pthread_t* tid_p = (pthread_t*) malloc(sizeof(pthread_t));
	(*tid_p) = tid;

	insert_entry_in_hash(conn_map_p->thread_id_to_file_discriptor, tid_P, fd_p);

	write_unlock(conn_map_p->thread_id_to_file_discriptor_lock);
}

int remove_mapping(connection_mapper* conn_map_p)
{
	return remove_mapping(conn_map_p, pthread_self());
}

int remove_mapping(connection_mapper* conn_map_p, pthread_t tid)
{
	write_lock(conn_map_p->thread_id_to_file_discriptor_lock);

	int entry_deleted = delete_entry_from_hash(conn_grp_p->thread_id_to_file_discriptor, &thread_id, &rkey, &rvalue);
	if(entry_deleted)
	{
		if(rkey != NULL)
		{free((void*)rkey);}
		if(rvalue != NULL)
		{free((void*)rvalue);}
	}

	write_unlock(conn_map_p->thread_id_to_file_discriptor_lock);

	return entry_deleted;
}

void delete_entry_operation(pthread_t* thread_id_p, int* file_discriptor_p, const void* ap)
{
	free(thread_id_p);
	free(file_discriptor_p);
}

void delete_all_mappings(connection_mapper* conn_map_p)
{
	for_each_entry_in_hash(conn_grp_p->thread_id_to_file_discriptor, (void (*)(const void*, const void*, const void*))(delete_entry_operation), NULL);
}

void close_entry_operation(pthread_t* thread_id_p, int* file_discriptor_p, const void* ap)
{
	close(*tid_p);
}

void close_all_file_discriptors(connection_mapper* conn_map_p)
{
	for_each_entry_in_hash(conn_grp_p->thread_id_to_file_discriptor, (void (*)(const void*, const void*, const void*))(close_entry_operation), NULL);
}

void delete_connection_mapper(connection_mapper* conn_map_p)
{
	// delete all the elements of the thread_id_to_file_discriptor hashmap
	delete_all_mappings(conn_map_p);
	delete_hashmap(conn_grp_p->thread_id_to_file_discriptor);
	delete_rwlock(conn_grp_p->thread_id_to_file_discriptor_lock);
}
