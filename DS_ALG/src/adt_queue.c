/**
 * @file adt_queue.c
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2024-04-18
 * @version 1.0
 */

#include "common_def.h"

#include "adt_queue.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

static s16 QUEUE_destroy(Queue* qu);
static s16 QUEUE_reset(Queue* qu);
static s16 QUEUE_resize(Queue* qu, u16 new_size);
static u16 QUEUE_capacity(Queue* qu);
static u16 QUEUE_lenght(Queue* qu);
static boolean QUEUE_isEmpty(Queue* qu);
static boolean QUEUE_isFull(Queue* qu);
static s16 enqueue(Queue* qu, void* data, u16 bytes);//insertLast
static void* dequeue(Queue* qu);//extract first
static void* QUEUE_back(Queue* qu);//last
static void* QUEUE_front(Queue* qu);//first
static s16 QUEUE_concat(Queue* qu, Queue* qu_src);
static void QUEUE_print(Queue* stack);

struct queue_ops_s queue_ops = {

								.destroy = QUEUE_destroy,
								.reset = QUEUE_reset,
								.resize = QUEUE_resize,
								.capacity = QUEUE_capacity,
								.length = QUEUE_lenght,
								.isEmpty = QUEUE_isEmpty,
								.isFull = QUEUE_isFull,
								.enqueue = enqueue,
								.dequeue = dequeue,
								.back = QUEUE_back,
								.front = QUEUE_front,
								.concat = QUEUE_concat,
								.print = QUEUE_print,
};

Queue* QUEUE_create(u16 capacity)
{
	Queue* qu = (Queue*)malloc(sizeof(Queue));
	if (NULL == qu) 
	{
		return NULL;
	}
	qu->storage_ = LIST_create(capacity);
	if (NULL == qu->storage_)
	{
		free(qu);
		return NULL;
	}
	qu->ops_ = &queue_ops;

	return qu;
}

s16 QUEUE_destroy(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->destroy(qu->storage_);
	return kErrorCode_Ok;
}

s16 QUEUE_reset(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->destroy(qu->storage_);
	return kErrorCode_Ok;
}
s16 QUEUE_resize(Queue* qu, u16 new_size)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->resize(qu->storage_, new_size);
	return kErrorCode_Ok;
}
u16 QUEUE_capacity(Queue* qu) 
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return 0;
	}
	qu->storage_->ops_->capacity(qu->storage_);
	return kErrorCode_Ok;
}
u16 QUEUE_lenght(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return 0;
	}
	qu->storage_->ops_->length(qu->storage_);
	return kErrorCode_Ok;
}
boolean QUEUE_isEmpty(Queue* qu) 
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_StackNull;
	}
	return qu->storage_->ops_->isEmpty(qu->storage_);
}
boolean QUEUE_isFull(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_StackNull;
	}
	return qu->storage_->ops_->isFull(qu->storage_);

}
s16 enqueue(Queue* qu, void* data, u16 bytes) 
{
	if (NULL == qu || NULL == qu->storage_ ) 
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->insertLast(qu->storage_, data, bytes);
	return kErrorCode_Ok;

}//insertLast
void* dequeue(Queue* qu) 
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	return qu->storage_->ops_->extractFirst(qu->storage_);

}//extract first
void* QUEUE_back(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	return qu->storage_->ops_->last(qu->storage_);
}//last
void* QUEUE_front(Queue* qu) 
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	return qu->storage_->ops_->first(qu->storage_);

}//first
s16 QUEUE_concat(Queue* qu, Queue* qu_src) 
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->concat(qu->storage_, qu_src->storage_);
	return kErrorCode_Ok;
}
void QUEUE_print(Queue* qu)
{
	if (NULL == qu || NULL == qu->storage_)
	{
		return kErrorCode_QueueNull;
	}
	qu->storage_->ops_->print(qu->storage_);
	return kErrorCode_Ok;
}