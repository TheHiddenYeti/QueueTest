//
//  Queue.h
//  SuckerPunch
//
//  Created by Christian Helmich on 22.04.12.
//  Copyright (c) 2012 Christian Helmich. All rights reserved.
//

#ifndef SuckerPunch_Queue_h
#define SuckerPunch_Queue_h

/* Uncomment this code & comment out definitions in Queue.cpp to make Q class accessible to tests */
//typedef unsigned char	byteType;
//typedef unsigned short	uShort;
//
//struct queued_byte
//{
//	byteType value;
//	static uShort current_count;
//
//	static queued_byte* const begin();
//	static queued_byte* const end();
//	static queued_byte* const at(unsigned int idx);
//
//	static bool in_valid_range(queued_byte* qb);
//	static bool in_valid_range(unsigned int idx);
//	static bool memory_available();
//
//	void invalidate();
//};
//
//class Q
//{
//public:
//	uShort start_offset;	//can exceed 255
//	uShort length;			//can exceed 255
//
//	static uShort current_count;
//	static const uShort max_count;
//
//	static uShort current_max_length;
//	static const uShort max_bytesize;
//public:
//	//API replication
//	static Q* create();
//	static void destroy(Q* q);
//
//	void enqueue_byte(byteType b);
//	byteType dequeue_byte();
//
//	//queued bytes iterating
//	queued_byte* const queued_bytes_begin();
//	queued_byte* const queued_bytes_end();
//	queued_byte* const queued_byte_at(unsigned int idx);
//
//	//Q iterating
//	static Q* const begin();
//	static Q* const end();
//	static uShort get_start_offset(Q* q);
//
//	//conditions to check for data validity
//	static bool at_least_one_exists();
//	bool in_valid_range();
//	bool is_valid();
//	bool length_is_valid();
//	bool offset_is_valid();
//
//	uShort get_queued_bytes_data_size();
//
//protected:
//	void destroy_queued_bytes();
//	void move_queued_bytes(queued_byte* const targetBuffer);
//	void shift_left_queued_bytes();
//
//	void bound_check_and_memory_rearrange();
//};

class Q;

Q* create_queue();			//Creates a FIFO byte queue, returning a handle to it.
void destroy_queue(Q* q);	//Destroy an earlier created byte queue.
void enqueue_byte(Q* q, unsigned char b);	//Adds a new byte to a queue.
unsigned char dequeue_byte(Q* q);			//Pops the next byte off the FIFO queue.

void on_out_of_memory();
void on_illegal_operation();


#endif
