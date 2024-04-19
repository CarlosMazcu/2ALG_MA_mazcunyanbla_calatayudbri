/**
 * @file adt_queue.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2024-04-19
 * @version 1.0
 */


#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__

#include "EDK_MemoryManager/edk_platform_types.h"
#include "adt_list.h"

typedef struct queue_s
{
	List* storage;
	struct queue_ops_s ops_;
} Queue;

struct queue_ops_s
{
	s16(*destroy)(Queue* qu);
	s16(*reset)(Queue* qu);
	s16(*resize)(Queue* qu, u16 new_size);
	u16(*capacity)(Queue* qu);
	u16(*length)(Queue* qu);
	boolean(*isEmpty)(Queue* qu);
	boolean(*isFull)(Queue* qu);
	s16 (*enqueue)(Queue* qu, void* data, u16 bytes);//insertLast
	void* (*dequeue)(Queue *qu);//extract first
	void* (*back)(Queue *qu);//last
	void* (*front)(Queue *qu);//first
	s16(*concat)(Queue* qu, Queue* qu_src);
	void (*print)(Queue* stack);




};

Queue* QUEUE_create(u16 capacity);

#endif // __ADT_QUEUE_H__