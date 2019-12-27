#include<connection_mapping.h>

// utility functions for the hashmap used in connection_mapping ---->>>>

unsigned long long int thread_id_hash(const int* thread_id)
{
	return ((unsigned long long int)(*thread_id));
}

int compare_thread_ids(const int* a, const int* b)
{
	return ((*a) - (*b));
}

// utility functions for the hashmap used in connection_mapping <<<<----

connection_mapper* get_connection_mapper()
{
	conn_grp_p->thread_id_to_file_discriptor = get_hashmap(10, (unsigned long long int (*)(const void* key))(thread_id_hash), (int (*)(const void* key1, const void* key2))(compare_thread_ids), ELEMENTS_AS_RED_BLACK_BST);
	conn_grp_p->thread_id_to_file_discriptor_lock = get_rwlock();
}

void insert(connection_mapper* conn_map_p, int* fd)
{

}

void insert_mapping(connection_mapper* conn_map_p, pthread_t* tid, int* fd)
{

}

void remove_mapping(connection_mapper* conn_map_p)
{

}

void remove_mapping(connection_mapper* conn_map_p, pthread_t* tid)
{

}

void delete_connection_mapper(connection_mapper* conn_map_p)
{

}
