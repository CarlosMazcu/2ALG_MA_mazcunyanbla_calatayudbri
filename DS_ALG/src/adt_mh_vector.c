/**
 * @file adt_vector.c
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "adt_mh_vector.h"
#include "common_def.h"

#include "EDK_MemoryManager/edk_memory_manager.h"
// Static prototipes
static s16 Mh_VECTOR_destroy(Mh_Vector* vector);//
static s16 Mh_VECTOR_softReset(Mh_Vector* vector);//
static s16 Mh_VECTOR_reset(Mh_Vector* vector);//
static s16 Mh_VECTOR_resize(Mh_Vector* vector, u16 new_capacity);//
static u16 Mh_VECTOR_capacity(Mh_Vector* vector);//
static u16 Mh_VECTOR_length(Mh_Vector* vector);//
static boolean Mh_VECTOR_isEmpty(Mh_Vector* vector);//
static boolean Mh_VECTOR_isFull(Mh_Vector *vector);//
static void* Mh_VECTOR_first(Mh_Vector *vector);//
static void* Mh_VECTOR_last(Mh_Vector *vector);//
static void* Mh_VECTOR_at(Mh_Vector *vector, u16 position);//
static s16 Mh_VECTOR_insertFirst(Mh_Vector *vector, void *data, u16 bytes);//
static s16 Mh_VECTOR_insertLast(Mh_Vector* vector, void *data, u16 bytes);//
static s16 Mh_VECTOR_insertAt(Mh_Vector *vector, void *data, u16 bytes, u16 position);//
static void* Mh_VECTOR_extractFirst(Mh_Vector* vector);//
static void* Mh_VECTOR_extractLast(Mh_Vector* vector);//
static void* Mh_VECTOR_extractAt(Mh_Vector* vector, u16 position);//
static s16 Mh_VECTOR_concat(Mh_Vector* vector, Mh_Vector *vector_src);
static s16 Mh_VECTOR_traverse(Mh_Vector* vector, void (*callback)(MemoryNode *));//
static void Mh_VECTOR_print(Mh_Vector* vector);

// vector´s api definitions
struct mh_vector_ops_s vector_ops = {
    .destroy = Mh_VECTOR_destroy,
    .softReset = Mh_VECTOR_softReset,
    .reset = Mh_VECTOR_reset,
    .resize = Mh_VECTOR_resize,
    .capacity = Mh_VECTOR_capacity,
    .length = Mh_VECTOR_length,
    .isEmpty = Mh_VECTOR_isEmpty,
    .isFull = Mh_VECTOR_isFull,
    .first = Mh_VECTOR_first,
    .last = Mh_VECTOR_last,
    .at = Mh_VECTOR_at,
    .insertFirst = Mh_VECTOR_insertFirst,
    .insertLast = Mh_VECTOR_insertLast,
    .insertAt = Mh_VECTOR_insertAt,
    .extractFirst = Mh_VECTOR_extractFirst,
    .extractLast = Mh_VECTOR_extractLast,
    .extractAt = Mh_VECTOR_extractAt,
    .concat = Mh_VECTOR_concat,
    .traverse = Mh_VECTOR_traverse,
    .print = Mh_VECTOR_print,
};

//reimplementado

Mh_Vector *Mh_VECTOR_create(u16 capacity)
{
  if(0 >= capacity)
  {
    return NULL;
  }
  Mh_Vector *vector_ = MM->malloc(sizeof(Mh_Vector));
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
  for(int i = 0; i < capacity * 2; i++)
  {
    MEMNODE_createLite(&vector_->storage_[i]);
  }
  vector_->head_ = capacity/4;
  if(0 != vector_->head_ % 4)
  {
    vector_->head_+=1;
  }
  vector_->tail_ = vector_->head_;
  vector_->capacity_ = capacity;
  vector_->ops_ = &vector_ops;
  return vector_;
}           
//reimplementado
s16 Mh_VECTOR_destroy(Mh_Vector* vector)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }

  if(NULL == vector->storage_)
  {
    MM->free(vector);
    return kErrorCode_Ok;
  }
  
  for(u16 i = vector->head_; i < vector->capacity_ * 2; i++)
  {
    if(NULL!= vector->storage_[i].data_)
    { 
      vector->storage_[i].ops_->reset(&vector->storage_[i]);
    }
  }

  MM->free(vector->storage_);
  MM->free(vector);

  return kErrorCode_Ok;
}


s16 Mh_VECTOR_softReset(Mh_Vector *vector)
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

  return kErrorCode_Ok;
}

s16 Mh_VECTOR_reset(Mh_Vector *vector)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if (NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  for (s16 i = vector->head_; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->reset(&vector->storage_[i]);
  }
  vector->head_ = 0;
  vector->tail_ = 0; 

  return kErrorCode_Ok;
}

u16 Mh_VECTOR_capacity(Mh_Vector *vector)
{
  if(NULL == vector)
  {
    return 0;
  }

  return vector->capacity_;

}

u16 Mh_VECTOR_length(Mh_Vector *vector)
{
  if (NULL == vector)
  {
    return 0;
  }

  return (vector->tail_ - vector->head_);
}

boolean Mh_VECTOR_isEmpty(Mh_Vector *vector)
{
  if (NULL != vector && vector->head_ == vector->tail_)
  {
    return True;
  }
  else
  {
    return False;
  }
}

boolean Mh_VECTOR_isFull(Mh_Vector *vector)
{

  if (NULL != vector && vector->tail_ >= vector->capacity_)
  {
    return True;

  }else{
    
    return False;

  }
}

s16 Mh_VECTOR_insertFirst(Mh_Vector *vector, void *data, u16 bytes)
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
  if(True == Mh_VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
 
  for (u16 i = vector->tail_; i > vector->head_; i--)
  {
    vector->storage_[0].ops_->setData(&vector->storage_[i], vector->storage_[i - 1].data_, vector->storage_[i - 1].size_);
  }

  vector->storage_[vector->head_].ops_->setData(&vector->storage_[vector->head_], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 Mh_VECTOR_insertLast(Mh_Vector *vector, void *data, u16 bytes)
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
  if (True == Mh_VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
  
  vector->storage_[vector->tail_].ops_->setData(&vector->storage_[vector->tail_], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 Mh_VECTOR_insertAt(Mh_Vector *vector, void *data, u16 bytes, u16 position)
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
  if (True == Mh_VECTOR_isFull(vector))
  {
    return kErrorCode_VectorFull;
  }
  if (position >= vector->tail_)
  {
    return Mh_VECTOR_insertLast(vector, data, bytes);
  }
  for (u16 i = vector->tail_; i > position; i--)
  {
    vector->storage_[i].ops_->setData(&vector->storage_[i], vector->storage_[i - 1].data_, vector->storage_[i - 1].size_); 
  }
  vector->storage_[position].ops_->setData(&vector->storage_[position], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

void *Mh_VECTOR_extractFirst(Mh_Vector *vector)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }
  if(True == Mh_VECTOR_isEmpty(vector))
  {
    return NULL;
  }
  void* tmp = vector->storage_[vector->head_].data_;
  for(int i = vector->head_; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->setData(&vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_);
  }
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;

  return tmp;
}

void *Mh_VECTOR_extractLast(Mh_Vector *vector)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }
  if (1 == Mh_VECTOR_isEmpty(vector))
  {
    return NULL;
  }

  void *tmp = vector->storage_[vector->tail_ - 1].data_;
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;

  return tmp;
}

void *Mh_VECTOR_extractAt(Mh_Vector *vector, u16 position)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }
  if (True == Mh_VECTOR_isEmpty(vector))
  {
    return NULL;
  }
  if(position >= vector->tail_)
  {
    return NULL;
  }

  void *tmp = vector->storage_[position].data_;

  for (int i = position; i < vector->tail_; i++)
  {
    vector->storage_[i].ops_->setData(&vector->storage_[i], vector->storage_[i + 1].data_, vector->storage_[i + 1].size_);
  }
  vector->storage_[vector->tail_ - 1].ops_->softReset(&vector->storage_[vector->tail_ - 1]);
  vector->tail_--;
  return tmp;
}

void *Mh_VECTOR_first(Mh_Vector *vector)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }

  if (NULL == vector->storage_[vector->head_].data_)
  {

    return NULL;
  }

  return vector->storage_[vector->head_].data_; 
}

void *Mh_VECTOR_last(Mh_Vector *vector)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }
  if(NULL == vector->storage_[vector->tail_ - 1].data_)
  {
    return NULL;
  }

  return vector->storage_[vector->tail_ - 1].data_;
}

void *Mh_VECTOR_at(Mh_Vector *vector, u16 position)
{
  if (NULL == vector)
  {
    return NULL;
  }
  if (NULL == vector->storage_)
  {
    return NULL;
  }
  if (position >= vector->tail_ || position >=vector->capacity_ || position <= vector->head_)
  {
    return NULL;
  }

  return vector->storage_[position].data_;
}

s16 Mh_VECTOR_traverse(Mh_Vector *vector, void (*callback)(MemoryNode *))
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if(NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }
  for(s16 i = vector->head_; i < vector->tail_; i++)
  {
    callback(&vector->storage_[i]);
  }

  return kErrorCode_Ok;
}

s16 Mh_VECTOR_resize(Mh_Vector *vector, u16 new_capacity)
{
  if(NULL == vector)
  {
    return kErrorCode_VectorNull;
  }
  if(NULL == vector->storage_)
  {
    return kErrorCode_StorageNull;
  }

  if(0 == new_capacity)
  {
    return kErrorCode_SizeZero;
  }

  MemoryNode *storage_tmp = (MemoryNode *)MM->malloc(sizeof(MemoryNode) * new_capacity);

  if(NULL == storage_tmp)
  {
    return kErrorCode_VectorNull;
  }
  
  if(new_capacity == vector->capacity_)
  {
    return kErrorCode_Ok;
  }
  //copy of storage in temporal storage with resize
  if(new_capacity > vector->capacity_)
  {
    for(int i = 0; i < vector->tail_; i++)
    {
      MEMNODE_createLite(&storage_tmp[i]);
      storage_tmp[i].ops_->setData(&storage_tmp[i], vector->storage_[i].data_, vector->storage_[i].size_);
    }

    for(int i = vector->tail_; i < new_capacity; i++)
    {
      MEMNODE_createLite(&storage_tmp[i]);
    } 
  }else if(new_capacity < vector->capacity_)
  {
    for (int i = 0; i < new_capacity; i++)
    {
      MEMNODE_createLite(&storage_tmp[i]);
      storage_tmp[i].ops_->setData(&storage_tmp[i], vector->storage_[i].data_, vector->storage_[i].size_);
    }

    for(int i = new_capacity; i < vector->tail_; i++)
    {
      storage_tmp->ops_->reset(&vector->storage_[i]);
    }
    vector->tail_ = new_capacity;
  }
  //free old storage
  MM->free(vector->storage_);


  vector->capacity_ = new_capacity;
  vector->storage_ = storage_tmp;

  return kErrorCode_Ok;
}

s16 Mh_VECTOR_concat(Mh_Vector* vector, Mh_Vector *vector_src)
{
  if(NULL == vector || NULL == vector_src)
  {
    return kErrorCode_VectorEmpty;
  }
  
  if (NULL == vector->storage_ || NULL == vector_src->storage_)
  {
    return kErrorCode_StorageNull;
  }

  MemoryNode *aux = (MemoryNode *)MM->malloc(sizeof(MemoryNode) * (vector->capacity_ + vector_src->capacity_));
  if (NULL == aux)
  {
    return kErrorCode_NodeNull;
  }
  
  for (u16 i = 0; i < vector->tail_; i++)
  {
    MEMNODE_createLite(&aux[i]);
    aux[i].ops_->setData(&aux[i],vector->storage_[i].data_,vector->storage_[i].size_);
  }
  
  for(u16 i = 0; i < vector_src->tail_; i++)
  {
    MEMNODE_createLite(&aux[i + vector->tail_]);
    aux[i].ops_->memCopy(&aux[i + vector->tail_], vector_src->storage_[i].data_, vector_src->storage_[i].size_);
   
  }
    vector->capacity_ += vector_src->capacity_;
    vector->tail_ += vector_src->tail_;

    MM->free(vector->storage_);

    vector->storage_ = aux;

    
  return kErrorCode_Ok; 
}

void Mh_VECTOR_print(Mh_Vector* vector)
{
  if(NULL == vector)
  {
    return;
  }
  printf("[VECTOR INFO] Adress: %p\n", vector);
  printf("[VECTOR INFO] Head: %d\n", vector->head_);
  printf("[VECTOR INFO] Tail: %d\n", vector->tail_);
  printf("[VECTOR INFO] Lenght: %d\n", vector->tail_);
  printf("[VECTOR INFO] Capacity: %d\n", vector->capacity_);
  if(NULL == vector->storage_)
  {
    return;
  }
  for(int i = vector->head_; i < vector->tail_; i++)
  {
    printf(" [VECTOR INFO] Storage #%d\n",i);
    printf("  [NODE INFO] Adress: %p\n", vector->storage_[i]);
    printf("  [NODE INFO] Size: %d\n", vector->storage_[i].size_);
     printf("  [NODE INFO] Data content:");
    for(int j = 0; j < vector->storage_[i].size_; j++)
    {
      printf("%c", *((char*)(vector->storage_[i].data_) + j));
      
    }
    printf("\n");
  }
  printf("\n");
  printf("\n");
}