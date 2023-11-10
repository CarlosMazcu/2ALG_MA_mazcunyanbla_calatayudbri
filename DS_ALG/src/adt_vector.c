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
static s16 VECTOR_destroy(Vector* vector);
static s16 VECTOR_softReset(Vector* vector);
static s16 VECTOR_reset(Vector* vector);
static s16 VECTOR_resize(Vector* vector, u16 new_capacity);
static u16 VECTOR_capacity(Vector* vector);
static u16 VECTOR_length(Vector* vector);
static boolean VECTOR_isEmpty(Vector* vector);
static boolean VECTOR_isFull(Vector *vector);
static void* VECTOR_first(Vector *vector);
static void* VECTOR_last(Vector *vector);
static void* VECTOR_at(Vector *vector, u16 position);
static s16 VECTOR_insertFirst(Vector *vector, void *data, u16 bytes);
static s16 VECTOR_insertLast(Vector* vector, void *data, u16 bytes);
static s16 VECTOR_insertAt(Vector *vector, void *data, u16 bytes, u16 position);
static void* VECTOR_extractFirst(Vector* vector);
static void* VECTOR_extractLast(Vector* vector);
static void* VECTOR_extractAt(Vector* vector, u16 position);
static s16 VECTOR_concat(Vector* vector, Vector *vector_src);
static s16 VECTOR_traverse(Vector* vector, void (*callback)(MemoryNode *));
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

}

s16 VECTOR_destroy(Vector* vector)
{

}

static s16 VECTOR_softReset(Vector *vector)
{

}

static s16 VECTOR_reset(Vector *vector)
{

}

static s16 VECTOR_resize(Vector *vector, u16 new_capacity)
{

}

static u16 VECTOR_capacity(Vector *vector)
{

}

static u16 VECTOR_length(Vector *vector)
{

}

static boolean VECTOR_isEmpty(Vector *vector)
{

}

static boolean VECTOR_isFull(Vector *vector)
{

}



/* · · · · <===3 
   8===D · · · ·*/