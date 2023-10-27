// adt_memory_node.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

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



// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .size = MEMNODE_size,
                                             .setData = MEMNODE_setData,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
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

s16 MEMNODE_createFromRef(MemoryNode **node) {
  *node = MEMNODE_create();
  if (NULL == *node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return kErrorCode_Memory;
  }
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
    return NULL;
  } 
  return node->data_;
}

u16	MEMNODE_size(MemoryNode *node) { // returns data size
  if (NULL == node)
  {
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
  node->data_ = src;
  node->size_ = bytes;
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


    return kErrorCode_Ok;
}