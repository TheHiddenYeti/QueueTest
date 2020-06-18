//
//  QueueTest.cpp
//
//  Sam Schroeder
//  6/10/2020
//

#include <cstdio>
#include <stdexcept>
#include "gtest/gtest.h"
#include "Queue.h"
#include "QueueTest.h"

//TODO: Data rearrangement stress test...? Based on 15 Qs w/ 80 bytes average?

/* Tests */

//Verify enqueue_byte() called with a null Q pointer calls on_illegal_operation()
TEST(QTest, EnqNullQIsIllegal) {
  Q* q = NULL;
  expect_illegal_enq(q);
}

//Verify dequeue_byte() called with a null Q pointer calls on_illegal_operation()
TEST(QTest, DeqNullQIsIllegal) {
  Q* q = NULL;
  expect_illegal_deq(q);
}

//Verify destroy_queue() called with a null Q pointer calls on_illegal_operation()
TEST(QTest, DestroyNullQIsIllegal) {
  Q* q = NULL;
  expect_illegal_destroy(q);
}

//Verify enqueue_byte() called with a pointer to a destroyed Q calls on_illegal_operation()
TEST(QTest, EnqDestroyedQIsIllegal) {
  Q* q = create_queue();
  destroy_queue(q);
  expect_illegal_enq(q);
}

//Verify dequeue_byte() called with a pointer to a destroyed Q calls on_illegal_operation()
TEST(QTest, DeqDestroyedQIsIllegal) {
  Q* q = create_queue();
  destroy_queue(q);
  expect_illegal_deq(q);
}

//Verify destroy_queue() called with a pointer to a destroyed Q calls on_illegal_operation()
TEST(QTest, DestroyDestroyedQIsIllegal) {
  Q* q = create_queue();
  destroy_queue(q);
  expect_illegal_destroy(q);
}

/*
This test FAILS with a crash due to access violation occurring
The KageKirin Q implementation does not call on_illegal_operation() in this case
*/
//Verify dequeue_byte() called with a pointer to an empty Q calls on_illegal_operation()
//TEST(QTest, DeqEmptyQIsIllegal) {
//  Q* q = create_queue();
//  try {
//    dequeue_byte(q);
//    FAIL() << "Expected on_illegal_operation() to throw std::runtime_error";
//  }
//  catch (const std::runtime_error& e) {
//    EXPECT_STREQ(e.what(), illegal_op_str);
//  }
//  catch (...) {
//    FAIL() << "Expected on_illegal_operation() to throw std::runtime_error";
//  }
//}

//Verify a single byte can be queued and immediately dequeued
TEST(QTest, SingleByteEnqDeqWorks) {
  Q* q = create_queue();
  enqueue_byte(q, 'a');
  EXPECT_EQ(dequeue_byte(q), 'a');
  destroy_queue(q);
}

//Verify a Q is FIFO (first in, first out)
TEST(QTest, QIsFifo) {
  Q* q = create_queue();
  for (int i = 1; i <= 5; ++i) {
    enqueue_byte(q, i);
  }
  for (int i = 1; i <= 5; ++i) {
    EXPECT_EQ(dequeue_byte(q), i);
  }
  destroy_queue(q);
}

//Verify multiple bytes can be queued and dequeued in FIFO fashion using multiple active Qs
TEST(QTest, EnqDeqWorks) {
  Q* q1 = create_queue();
  Q* q2 = create_queue();
  unsigned char byte = 0;
  enqueue_byte(q1, 1);
  enqueue_byte(q2, 3);
  enqueue_byte(q1, 2);
  byte = dequeue_byte(q1);
  EXPECT_EQ(byte, 1);
  enqueue_byte(q1, 5);
  enqueue_byte(q1, 6);
  enqueue_byte(q2, 4);
  enqueue_byte(q2, 7);
  byte = dequeue_byte(q1);
  EXPECT_EQ(byte, 2);
  byte = dequeue_byte(q2);
  EXPECT_EQ(byte, 3);
  byte = dequeue_byte(q2);
  EXPECT_EQ(byte, 4);
  enqueue_byte(q2, 8);
  byte = dequeue_byte(q1);
  EXPECT_EQ(byte, 5);
  byte = dequeue_byte(q1);
  EXPECT_EQ(byte, 6);
  destroy_queue(q1);
  byte = dequeue_byte(q2);
  EXPECT_EQ(byte, 7);
  byte = dequeue_byte(q2);
  EXPECT_EQ(byte, 8);
  destroy_queue(q2);
}

/* Assuming CANNOT get sizeof(Q) */
//Verify empty Qs can be destroyed and have their memory freed
TEST(QTest, DestroyEmptyQWorks) {
  //Minimum obj size is 1 byte, so there should not be more than 2048 Q objects allowed at once
  Q* qs[data_size+1];
  bool on_out_of_memory_called = false;
  int num_qs = 0;
  int prev_num_qs = 0;
  for (int n = 0; n < 2; ++n) {
    for (int i = 0; i < data_size + 1; ++i) {
      try {
        qs[i] = create_queue();
      }
      catch (const std::runtime_error& e) {
        num_qs = i;
        on_out_of_memory_called = true;
        break;
      }
      catch (...) {
        FAIL() << expected_out_of_mem_msg;
        break;
      }
    }

    EXPECT_TRUE(on_out_of_memory_called);
    on_out_of_memory_called = false;

    for (int i = 0; i < num_qs; ++i) {
      destroy_queue(qs[i]);
    }

    //Verify we can create the same # of Qs after destroying the previous set & not lose memory
    if (prev_num_qs > 0) {
      EXPECT_EQ(num_qs, prev_num_qs);
    }

    prev_num_qs = num_qs;
  }
}

///* Assuming CAN get sizeof(Q) */
////Verify empty Qs can be destroyed and have their memory freed
//TEST(QTest, DestroyEmptyQWorks) {
//  const int max_empty_qs = data_size / sizeof(Q);  //Max number of empty Qs that should fit in allowed memory
//  Q* qs[max_qs];
//
//  const int num_loops = (max_empty_qs / max_qs) + 1;
//
//  for (int i = 0; i < num_loops; ++i) {
//    for (int j = 0; j < max_qs; ++j) {
//      qs[j] = create_queue();
//    }
//
//    for (int j = 0; j < max_qs; ++j) {
//      destroy_queue(qs[j]);
//    }
//  }
//}

/* Assuming CANNOT get sizeof(Q) */
//Verify full Qs can be destroyed and have their memory freed
TEST(QTest, DestroyFullQWorks) {
  Q* q = create_queue();
  int bytes_1 = fill_queue(q);
  EXPECT_GT(bytes_1, 0);
  EXPECT_LT(bytes_1, data_size);
  destroy_queue(q);
 
  q = create_queue();
  int bytes_2 = fill_queue(q);
  EXPECT_GT(bytes_2, 0);
  EXPECT_LT(bytes_2, data_size);
  destroy_queue(q);

  //Verify we can enqueue the same # of bytes after 1st Q is destroyed (no memory should be lost)
  EXPECT_EQ(bytes_1, bytes_2);
}

///* Assuming CAN get sizeof(Q) */
////Verify full Qs can be destroyed and have their memory freed
//TEST(QTest, DestroyFullQWorks) {
//  Q* q = create_queue();
//  int bytes_qd_1 = fill_queue(q);
//  EXPECT_GT(bytes_qd_1, 0);
//  EXPECT_LE(bytes_qd_1 + sizeof(Q), data_size);
//  destroy_queue(q);
//
//  q = create_queue();
//  int bytes_qd_2 = fill_queue(q);
//  EXPECT_GT(bytes_qd_2, 0);
//  EXPECT_LE(bytes_qd_2 + sizeof(Q), data_size);
//  destroy_queue(q);
//
//  EXPECT_EQ(bytes_qd_1, bytes_qd_2);
//}

//Verify a Q can be created, filled, and have all bytes dequeued without memory loss
TEST(QTest, DequeueFullQWorks) {
  Q* q = create_queue();
  int bytes_1 = fill_queue(q);
  EXPECT_GT(bytes_1, 0);
  EXPECT_LT(bytes_1, data_size);

  for (int i = 0; i < bytes_1; ++i) {
    dequeue_byte(q);
  }

  int bytes_2 = fill_queue(q);
  EXPECT_GT(bytes_2, 0);
  EXPECT_LT(bytes_2, data_size);
  destroy_queue(q);

  //Verify we can enqueue the same # of bytes both times (no memory should be lost)
  EXPECT_EQ(bytes_1, bytes_2);
}

///* Assuming CAN get sizeof(Q) */
////Verify on_out_of_memory() is called when memory usage of a Q + its data would exceed 2048 bytes
//TEST(QTest, OutOfMemHandledSingleQ) {
//  Q* q = create_queue();
//  int bytes_qd = fill_queue(q);
//  EXPECT_GT(bytes_qd, 0);
//  EXPECT_LE(bytes_qd + sizeof(Q), data_size);
//  destroy_queue(q);
//}
//
///* Assuming CAN get sizeof(Q) */
////Verify on_out_of_memory() is called when memory usage of multiple Qs and all data would exceed 2048 bytes
//TEST(QTest, OutOfMemHandledManyQs) {
//  Q* qs[max_qs];
//
//  //Test with all number of active queues from 2 to 15
//  for (int n = 2; n <= max_qs; ++n) {
//    for (int i = 0; i < n; ++i) {
//      qs[i] = create_queue();
//    }
//
//    //Enqueue a byte to each active queue in round-robin fashion until on_out_of_memory() is called
//    int index = 0;
//    int bytes_qd = 0;
//    while (bytes_qd <= data_size) {
//      index = bytes_qd % n;
//      try {
//        enqueue_byte(qs[index], n % 255);
//        ++bytes_qd;
//      }
//      catch (const std::runtime_error& e) {
//        EXPECT_STREQ(e.what(), out_of_memory_str);
//        break;
//      }
//      catch (...) {
//        FAIL() << expected_out_of_mem_msg;
//        break;
//      }
//    }
//
//    EXPECT_GT(bytes_qd, 0);
//    EXPECT_LE(bytes_qd + (sizeof(Q) * n), data_size);
//
//    for (int i = 0; i < n; ++i) {
//      destroy_queue(qs[i]);
//    }
//  }
//}

//Verify 15 Qs each containing 80 bytes can be created and have their bytes dequeued
TEST(QTest, EqualBytesMaxQsSupported) {
  const int bytes_per_queue = avg_bytes;
	Q* qs[max_qs];

	for (int i = 0; i < max_qs; ++i) {
    qs[i] = create_queue();
    for (int j = 0; j < bytes_per_queue; ++j) {
      enqueue_byte(qs[i], (i * j) % 255);
    }
	}

	for (int i = 0; i < max_qs; ++i) {
		for (int j = 0; j < bytes_per_queue; ++j) {
		  EXPECT_EQ(dequeue_byte(qs[i]), (i * j) % 255);
		}
		destroy_queue(qs[i]);
	}
}

//Verify 15 Qs containing varying numbers of bytes can be created with an average of "80 or so" per Q
TEST(QTest, VaryBytesMaxQsSupported) {
  //1214 bytes / 15 queues == 80.93 avg (for testing high end of "80 or so" average)
  int max_bytes = (max_qs * avg_bytes) + max_qs - 1;
  int bytes_per_q[15] = {0};

  Q* qs[max_qs];

  srand(time(NULL));

  for (int i = 0; i < max_qs; ++i) {
    qs[i] = create_queue();

    if (i == max_qs - 1) {
      //Put all the leftover bytes in the last Q
      bytes_per_q[i] = max_bytes;
    }
    else {
      bytes_per_q[i] = rand() % (max_bytes / max_qs);
    }
   
    max_bytes -= bytes_per_q[i];

    for (int j = 0; j < bytes_per_q[i]; ++j) {
      enqueue_byte(qs[i], (i * j) % 255);
    }
  }

  for (int i = 0; i < max_qs; ++i) {
    for (int j = 0; j < bytes_per_q[i]; ++j) {
      EXPECT_EQ(dequeue_byte(qs[i]), (i * j) % 255);
    }
    destroy_queue(qs[i]);
  }
}

/* Test sub-routines */

void expect_illegal_enq(Q* q) {
  try {
    enqueue_byte(q, 0);
    FAIL() << expected_illegal_op_msg;
  }
  catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), illegal_op_str);
  }
  catch (...) {
    FAIL() << expected_illegal_op_msg;
  }
}

void expect_illegal_deq(Q* q) {
  try {
    dequeue_byte(q);
    FAIL() << expected_illegal_op_msg;
  }
  catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), illegal_op_str);
  }
  catch (...) {
    FAIL() << expected_illegal_op_msg;
  }
}

void expect_illegal_destroy(Q* q) {
  try {
    destroy_queue(q);
    FAIL() << expected_illegal_op_msg;
  }
  catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), illegal_op_str);
  }
  catch (...) {
    FAIL() << expected_illegal_op_msg;
  }
}

//Returns # of bytes queued before on_out_of_memory() was called, or -1 if Q could not be filled as expected
int fill_queue(Q* q) {
  bool on_out_of_memory_called = false;
  int i = 0;
  while (i <= data_size) {
    try {
      enqueue_byte(q, i % 255);
      ++i;
    }
    catch (const std::runtime_error& e) {
      if (strcmp(e.what(), out_of_memory_str) == 0) {
        on_out_of_memory_called = true;
      }
      break;
    }
    catch (...) {
      return -1;
    }
  }

  if (on_out_of_memory_called) {
    return i;
  }
  else {
    return -1;
  }
}
