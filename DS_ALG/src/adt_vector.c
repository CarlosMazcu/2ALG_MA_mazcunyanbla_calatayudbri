// adt_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "adt_vector.h"
#include "common_def.h"

#include "EDK_MemoryManager/edk_memory_manager.h"
// Static prototipes
static s16 VECTOR_destroy(Vector* vector);//
static s16 VECTOR_softReset(Vector* vector);//
static s16 VECTOR_reset(Vector* vector);//
static s16 VECTOR_resize(Vector* vector, u16 new_capacity);//
static u16 VECTOR_capacity(Vector* vector);//
static u16 VECTOR_length(Vector* vector);//
static boolean VECTOR_isEmpty(Vector* vector);//
static boolean VECTOR_isFull(Vector *vector);//
static void* VECTOR_first(Vector *vector);//
static void* VECTOR_last(Vector *vector);//
static void* VECTOR_at(Vector *vector, u16 position);//
static s16 VECTOR_insertFirst(Vector *vector, void *data, u16 bytes);//
static s16 VECTOR_insertLast(Vector* vector, void *data, u16 bytes);//
static s16 VECTOR_insertAt(Vector *vector, void *data, u16 bytes, u16 position);//
static void* VECTOR_extractFirst(Vector* vector);//
static void* VECTOR_extractLast(Vector* vector);//
static void* VECTOR_extractAt(Vector* vector, u16 position);//
static s16 VECTOR_concat(Vector* vector, Vector *vector_src);
static s16 VECTOR_traverse(Vector* vector, void (*callback)(MemoryNode *));//
static void VECTOR_print(Vector* vector);

// vector´s api definitions
struct vector_ops_s vector_ops = {
    .destroy = VECTOR_destroy,
    .softReset = VECTOR_softReset,
    .reset = VECTOR_reset,
    .resize = VECTOR_resize,
    .capacity = VECTOR_capacity,
    .length = VECTOR_length,
    .isEmpty = VECTOR_isEmpty,
    .isFull = VECTOR_isFull,
    .first = VECTOR_first,
    .last = VECTOR_last,
    .at = VECTOR_at,
    .insertFirst = VECTOR_insertFirst,
    .insertLast = VECTOR_insertLast,
    .insertAt = VECTOR_insertAt,
    .extractFirst = VECTOR_extractFirst,
    .extractLast = VECTOR_extractLast,
    .extractAt = VECTOR_extractAt,
    .concat = VECTOR_concat,
    .traverse = VECTOR_traverse,
    .print = VECTOR_print,
};

Vector *VECTOR_create(u16 capacity)
{
  if(0 >= capacity)
  {
    return NULL;
  }
  Vector *vector_ = MM->malloc(sizeof(Vector));
  if (NULL == vector_)
  {
    return NULL;
  }
  vector_->storage_ = MM->malloc(sizeof(MemoryNode) * capacity);
  if(NULL == vector_->storage_)
  {
    MM->free(vector_);
    return NULL;
  }
  for(int i = 0; i < capacity; i++)
  {
    MEMNODE_createLite(&vector_->storage_[i]);
  }
  vector_->head_ = 0;
  vector_->tail_ = 0;
  vector_->capacity_ = capacity;
  vector_->ops_ = &vector_ops;
  return vector_;
}

s16 VECTOR_destroy(Vector* vector)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }

  if(NULL == vector->storage_)
  {
    MM->free(vector);
    return kErrorCode_StorageNull;
  }
  
  for(u16 i = vector->head_; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->reset(&vector->storage_[i]);
  }
  MM->free(vector->storage_);
  MM->free(vector);

  return kErrorCode_Ok;
}

static s16 VECTOR_softReset(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  for (u16 i = vector->head_; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->softReset(&vector->storage_[i]);
  }
  vector->tail_ = 0;
}

static s16 VECTOR_reset(Vector *vector)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  for (u16 i = vector->head_; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->reset(&vector->storage_[i]);
  }
  vector->tail_ = 0; 
}

u16 VECTOR_capacity(Vector *vector)
{
  if(NULL == vector)
  {
    return 0;
  }

  return vector->capacity_;

}

u16 VECTOR_length(Vector *vector)
{
  if (NULL == vector)
  {
    return 0;
  }

  return (vector->tail_ - vector->head_);
}

boolean VECTOR_isEmpty(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (vector->head_ == vector->tail_)
  {
    return True;
  }
  else
  {
    return False;
  }
}

boolean VECTOR_isFull(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if(vector->tail_ == vector->capacity_)
  {
    return True;
  }else{
    return False;
  }
}

s16 VECTOR_insertFirst(Vector *vector, void *data, u16 bytes)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if(NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if (NULL == data)
  {
    return kErrorCode_SrcNull;
  }
  if (0 >= bytes)
  {
    return kErrorCode_BytesZero;
  }
  if(1 == VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
  for (u16 i = vector->tail_; i > vector->head_; i--)
  {
   /*  MEMNODE_setData(vector->storage_[i + 1], vector->storage_[i].data_, vector->storage_[i].size_); */
   vector->storage_[i + 1].ops_->setData(&vector->storage_[i + 1], vector->storage_[i].data_, vector->storage_[i].size_);
  }
  /* MEMNODE_setData(vector->storage_[vector->head_], data, bytes); */
  vector->storage_[vector->head_].ops_->setData(&vector->storage_[vector->head_], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 VECTOR_insertLast(Vector *vector, void *data, u16 bytes)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if (NULL == data)
  {
    return kErrorCode_SrcNull;
  }
  if (0 >= bytes)
  {
    return kErrorCode_BytesZero;
  }
  if (1 == VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
  /* MEMNODE_setData(vector->storage_[vector->tail_], data, bytes); */
  vector->storage_[vector->tail_].ops_->setData(&vector->storage_[vector->tail_], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 VECTOR_insertAt(Vector *vector, void *data, u16 bytes, u16 position)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if (NULL == data)
  {
    return kErrorCode_SrcNull;
  }
  if (0 >= bytes)
  {
    return kErrorCode_BytesZero;
  }
  if (1 == VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
  if (position >= vector->tail_)
  {
    VECTOR_insertLast(vector, data, bytes);
  }
  for (u16 i = vector->tail_; i > position; i--)
  {
    /* MEMNODE_setData(vector->storage_[i + 1], vector->storage_[i].data_, vector->storage_[i].size_); */
    vector->storage_[i + 1].ops_->setData(&vector->storage_[i + 1], vector->storage_[i].data_, vector->storage_[i].size_);
  }
  /* MEMNODE_setData(vector->storage_[position], data, bytes); */
  vector->storage_[position].ops_->setData(&vector->storage_[position], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

void *VECTOR_extractFirst(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if(1 == VECTOR_isEmpty(vector))
  {
    return kErrorCode_VectorEmpty;
  }
  void* tmp = vector->storage_[vector->head_].data_;
  for(int i = vector->head_; i > vector->tail_; i++)
  {
    /* MEMNODE_setData(vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_); */
    vector->storage_[i].ops_->setData(&vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_);
  }
  /* MEMNODE_softReset(vector->storage_[vector->tail_ - 1]); */
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;

  return tmp;
}

void *VECTOR_extractLast(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if (1 == VECTOR_isEmpty(vector))
  {
    return kErrorCode_VectorEmpty;
  }

  void *tmp = vector->storage_[vector->tail_ - 1].data_;
  /* MEMNODE_softReset(vector->storage_[vector->tail_ - 1]); */
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;

  return tmp;
}

void *VECTOR_extractAt(Vector *vector, u16 position)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  if (1 == VECTOR_isEmpty(vector))
  {
    return kErrorCode_VectorEmpty;
  }
  if(position >= vector->tail_)
  {
    return kErrorCode_PositionMismatch;
  }
  void *tmp = vector->storage_[position].data_;

  for (int i = position; i > vector->tail_; i++)
  {
    /* MEMNODE_setData(vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_); */
    vector->storage_[i].ops_->setData(&vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_);
  }
  /* MEMNODE_softReset(vector->storage_[vector->tail_ - 1]); */
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;
  return tmp;
}

void *VECTOR_first(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }

  return vector->storage_[vector->head_].data_; 
}

void *VECTOR_last(Vector *vector)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }

  return vector->storage_[vector->tail_ - 1].data_;
}

void *VECTOR_at(Vector *vector, u16 position)
{
  if (NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }

  return vector->storage_[position].data_;
}

s16 VECTOR_traverse(Vector *vector, void (*callback)(MemoryNode *))
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  for(s16 i = vector->head_; i < vector->tail_; i++)
  {
    callback(&vector->storage_[i]);
  }
}

s16 VECTOR_resize(Vector *vector, u16 new_capacity)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorFull;
  }
  if(NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  MemoryNode *storage_tmp = MM->malloc(sizeof(MemoryNode) * new_capacity);
  //copy of storage in temporal storage with resize
  for(int i = vector->head_; i < vector->tail_; i++)
  {
/*     MEMNODE_setData(storage_tmp[i], vector->storage_[i].data_, vector->storage_[i].size_); */

     storage_tmp[i].ops_->setData(&storage_tmp[i], vector->storage_[i].data_, vector->storage_[i].size_);

     
  }
  //free old storage
  MM->free(vector->storage_);

  vector->capacity_ = new_capacity;
  vector->storage_ = storage_tmp;

  return kErrorCode_Ok;
}

s16 VECTOR_concat(Vector* vector, Vector *vector_src)
{
  if(NULL == vector || NULL == vector_src)
  {
    return kErrorCode_VectorEmpty;
  }
  
  if(vector->storage_ < vector->tail_ + vector_src->tail_)
  {
    return kErrorCode_NotEnoughCapacity;
  }

  for(u16 i=0; i < vector_src->tail_; i++)
  {
    MemoryNode *aux = (MemoryNode *)MM->malloc(sizeof(MemoryNode));
    
    *aux = vector_src->storage_[i];

    vector->storage_[vector->tail_ + i] = *aux;
  }

    vector->tail_ += vector_src->tail_;

  return kErrorCode_Ok;
}


void VECTOR_print(Vector* vector)
{
  if(NULL == vector)
  {
    return NULL;
  }
  printf("[VECTOR INFO] Adress: %s\n", vector);
  printf("[VECTOR INFO] Head: %d\n", vector->head_);
  printf("[VECTOR INFO] Tail: %d\n", vector->tail_);
  printf("[VECTOR INFO] Lenght: %d\n", vector->tail_);
  printf("[VECTOR INFO] Capacity: %d\n", vector->capacity_);
  if(NULL == vector->storage_)
  {
    return NULL;
  }
  for(int i = vector->head_; i < vector->tail_; i++)
  {
    printf(" [VECTOR INFO] Storage #%d\n",i);
    printf("  [NODE INFO] Adress: %s\n", vector->storage_[i]);
    printf("  [NODE INFO] Size: %d\n", vector->storage_[i].size_);
    printf("  [NODE INFO] Data address: %s\n", vector->storage_[i].data_);
    printf("  [NODE INFO] Data content: %d\n", &vector->storage_[i].data_);
    printf("  [NODE INFO] Next address: %s\n", vector->storage_[i + 1]);
    printf("  [NODE INFO] Prev address: %s\n", vector->storage_[i - 1]);
  }

}