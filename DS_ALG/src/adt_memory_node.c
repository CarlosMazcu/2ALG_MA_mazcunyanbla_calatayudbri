/**
 * @file adt_memory_node.c
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_memory_node.h"

#include "EDK_MemoryManager/edk_memory_manager.h"


// Memory Node Declarations
static s16 MEMNODE_initWithoutCheck(MemoryNode *node);	// inits a MN with no checks
static void* MEMNODE_data(MemoryNode *node);	// returns a reference to data_
static u16 MEMNODE_size(MemoryNode *node);		// returns data size
static s16 MEMNODE_setData(MemoryNode* node, void* src, u16 bytes);

static s16 MEMNODE_reset(MemoryNode* node);
static s16 MEMNODE_softReset(MemoryNode* node);
static s16 MEMNODE_free(MemoryNode *node);
static s16 MEMNODE_softFree(MemoryNode *node);

static s16 MEMNODE_memSet(MemoryNode *node, u8 value);
static s16 MEMNODE_memCopy(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memMask(MemoryNode *node, u8 mask);

static void MEMNODE_print(MemoryNode *node);


// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .size = MEMNODE_size,
                                             .setData = MEMNODE_setData,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
                                             .free = MEMNODE_free,
                                             .softFree = MEMNODE_softFree,
                                             .memSet = MEMNODE_memSet,
                                             .memCopy = MEMNODE_memCopy,
                                             .memConcat = MEMNODE_memConcat,
                                             .memMask = MEMNODE_memMask,
                                             .print = MEMNODE_print,
};

// Memory Node Definitions
MemoryNode* MEMNODE_create() {
  MemoryNode *node = MM->malloc(sizeof(MemoryNode));
  if (NULL == node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return NULL;
  }
  MEMNODE_initWithoutCheck(node);
  return node;
}

s16 MEMNODE_createLite(MemoryNode *node)
{
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  node->data_ = NULL;
  node->size_ = 0;
  node->ops_ = &memory_node_ops;
  return kErrorCode_Ok;
}

s16 MEMNODE_createFromRef(MemoryNode **node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  *node = MEMNODE_create();
  if(NULL == *node)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
  }
  MEMNODE_initWithoutCheck(*node);
  return kErrorCode_Ok;
}

s16 MEMNODE_initWithoutCheck(MemoryNode *node) {
  node->data_ = NULL;
  node->size_ = 0;
  node->ops_ = &memory_node_ops;
  return kErrorCode_Ok;
}

void* MEMNODE_data(MemoryNode *node)
{ // returns a reference to data_
  if(NULL == node)
  {
  #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
  #endif
    return NULL;
  } 
  return node->data_;
}

u16	MEMNODE_size(MemoryNode *node) { // returns data size
  if (NULL == node)
  {
  #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
  #endif
    return 0;
  }
  return node->size_;
}

s16 MEMNODE_setData(MemoryNode *node, void *src, u16 bytes)
{
  if(NULL == node){
    return kErrorCode_NodeNull;
  }
  if(NULL == src){
    return kErrorCode_SrcNull;
  }
  if(0 == bytes){
    return kErrorCode_BytesZero;
  }
  
      node->size_ = bytes;
      node->data_ = src;
      return kErrorCode_Ok;
  
}

s16 MEMNODE_reset(MemoryNode* node) 
{
  if (NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if (NULL == node->data_)
  {
    return kErrorCode_DataNull;
  }
  
  MM->free(node->data_);
  node->data_ = NULL;
  node->size_ = 0;
  return kErrorCode_Ok;
}

s16 MEMNODE_softReset(MemoryNode* node)
{
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
  
  node->data_ = NULL;
  node->size_ = 0;
    return kErrorCode_Ok;
}

s16 MEMNODE_free(MemoryNode *node)
{
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }

  if (NULL == node->data_)
  {
    MM->free(node);
    return kErrorCode_Ok;
  }
  

    MM->free(node->data_);
    // node->data_ = NULL;
   // node->size_ = 0;

    MM->free(node);  
    // node = NULL;

  return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode *node)
{

  if (NULL == node)
  {
    return kErrorCode_NodeNull;
  }

  MM->free(node);
  //node = NULL;
  return kErrorCode_Ok;
}

s16 MEMNODE_memSet(MemoryNode *node, u8 value)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_)
  {
    return kErrorCode_DataNull;
  }
  if(0 == node->size_)
  {
    return kErrorCode_SizeZero;
  }
  memset(node->data_, value, node->size_);
  return kErrorCode_Ok;
}

s16 MEMNODE_memCopy(MemoryNode* node, void* src, u16 bytes)
{
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    if (NULL == src) {
        return kErrorCode_Null;
    }
    if (bytes == 0) {
        return kErrorCode_SizeZero;
    }
    if (NULL != node->data_) {
        MM->free(node->data_);
    }
    u8* aux = (u8*)src;
    u8* aux2 = (u8*)MM->malloc(bytes);
    if (NULL == aux2) {
        return kErrorCode_Memory;
    }
    for (int i = 0; i < bytes; i++)
    {
        aux2[i] = aux[i];
    }
    node->data_ = aux2;
    node->size_ = bytes;
    return kErrorCode_Ok;
}

s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_)
  {
    return kErrorCode_DataNull;
  }
  
  u16 aux_size = node->size_ + bytes;
  u8 *aux_1 = (u8*)node->data_;
  u8* aux_2 = (u8*)src;
  u8 *aux_3 = (u8*)MM->malloc(aux_size);

  if(NULL == aux_3)
  {
    return kErrorCode_Memory;
  }
  int i = 0;
  
  for(i = 0; i < node->size_; i++){
    aux_3[i] = (aux_1[i]);
  }
  for (i; i < aux_size; i++)
  {
    aux_3[i] = aux_2[i-node->size_];
  }

  MM->free(node->data_);
  node->data_ = aux_3;
  node->size_ = aux_size;

  return kErrorCode_Ok;
}

s16 MEMNODE_memMask(MemoryNode *node, u8 mask)
{
  if(NULL == node) {
    return kErrorCode_NodeNull;
  }

  if(NULL == node->data_){
    return kErrorCode_DataNull;
  }

  u8* aux = (u8*)node->data_;

  for (u8 i = 0; i < node->size_; i++) {
      aux[i] &= mask;
  }

  return kErrorCode_Ok;
}

void MEMNODE_print(MemoryNode *node)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  printf("[Node Info]Adress Node: %p\n", node);
  printf("[Node Info]Size: %d\n", node->size_);
  if(NULL == node->data_)
  {
   printf("\t[Node Info] Data Address: NULL\n");
   return kErrorCode_DataNull;
  }
  printf("\t[Node Info] Data Address: %p\n", node->data_);
  printf("\t[Node Info] Data Content: \n");
  u8* aux = (u8*)node->data_;
  for (int i = 0; i < node->size_; i++)
  {
      printf("%c", aux[i]);
  }
  printf("\n");
  return kErrorCode_Ok;
}
