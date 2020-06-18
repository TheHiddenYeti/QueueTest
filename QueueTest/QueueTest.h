//
//  QueueTest.h
//
//  Sam Schroeder
//  6/10/2020
//

const int data_size = 2048;  //Maximum number of bytes allowed to be used by all queues
const int max_qs = 15;  //Maximum number of Qs to be supported for existing at once
const int avg_bytes = 80;  //Average number of bytes per Q to be supported w/ 15 queues

const char* out_of_memory_str = "out of memory";  //Message in std::runtime_error thrown by on_out_of_memory()
const char* illegal_op_str = "illegal operation";  //Message in std::runtime_error thrown by on_illegal_operation()

const char* expected_illegal_op_msg = "Expected on_illegal_operation() to throw std::runtime_error, got different exception";
const char* expected_out_of_mem_msg = "Expected on_out_of_memory() to throw std::runtime_error, got different exception";

/* Test sub-routines */
void expect_illegal_enq(Q* q);  //Fail if on_illegal_operation() is not called on enqueue
void expect_illegal_deq(Q* q);  //Fail if on_illegal_operation() is not called on dequeue
void expect_illegal_destroy(Q* q);  //Fail if on_illegal_operation() is not called on Q destroy
int fill_queue(Q* q);  //Enqueue bytes until on_out_of_memory() is called
