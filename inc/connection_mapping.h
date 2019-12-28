#ifndef CONNECTION_MAPPING_H
#define CONNECTION_MAPPING_H

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#include<hashmap.h>
#include<rwlock.h>

// the connection_mapper is a construct, which stores mapping from
// a thread to a file discriptor, this mapping will help us remember which thread
// is responsible for serving what connection

typedef struct connection_mapper connection_mapper;
struct connection_mapper
{
	// the threads of connection_group, have to remember which connection they were working on
	// this hashmap is shared among the threads to access, their own file discriptors
	hashmap* thread_id_to_file_discriptor;
	rwlock* thread_id_to_file_discriptor_lock;
};

connection_mapper* get_connection_mapper(unsigned long long int expected_connections);

// returns the file discriptor, that was mapped with the given thread
int get_mapping(connection_mapper* conn_map_p, pthread_t tid);

// insert mapping between the given thread and the given file_discriptor fd
void insert_mapping(connection_mapper* conn_map_p, pthread_t tid, int fd);

// remove the mapping to the file_discriptor, of the given thread
// returns the number of mappings removed from the connection_mapper
int remove_mapping(connection_mapper* conn_map_p, pthread_t tid);

// removes all the mappings from the connection_mapper
void remove_all_mapping(connection_mapper* conn_map_p);

// close all the file discriptors that are present in the connection_mapper
void close_all_file_discriptors(connection_mapper* conn_map_p);

// delete the given connection mapping
void delete_connection_mapper(connection_mapper* conn_map_p);

#endif