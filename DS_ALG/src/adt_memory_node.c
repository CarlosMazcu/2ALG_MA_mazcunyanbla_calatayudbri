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
static s16 MEMMNODE_free(MemoryNode *node);
static s16 MEMMNODE_softFree(MemoryNode *node);

static s16 MEMMNODE_memSet(MemoryNode *node, u8 value);
static s16 MEMMNODE_memCopy(MemoryNode *node, void *src, u16 bytes);
static s16 MEMMNODE_memConcat(MemoryNode *node, void *src, u16 bytes);
static s16 MEMMNODE_memMask(MemoryNode *node, u8 mask);

static void MEMMNODE_print(MemoryNode *node);



// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .size = MEMNODE_size,
                                             .setData = MEMNODE_setData,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
                                             .free = MEMMNODE_free,
                                             .softFree = MEMMNODE_softFree,
                                             .memSet = MEMMNODE_memSet,
                                             .memCopy = MEMMNODE_memCopy,
                                             .memConcat = MEMMNODE_memConcat,
                                             .memMask = MEMMNODE_memMask,
                                             .print = MEMMNODE_print,
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
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
    return kErrorCode_NodeNull;
  }
  if(NULL == src){
    #ifdef VERBOSE_
    printf("Error: [%s] not data to copy in the memory node\n", __FUNCTION__);
    #endif
    return kErrorCode_SrcNull;
  }
  if(0 == bytes){
    #ifdef VERBOSE_
    printf("Error: [%s] bytes = 0, need not NULL value\n", __FUNCTION__);
    #endif
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
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
    return kErrorCode_NodeNull;
  }
  if (NULL == node->data_)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] node data NULL\n", __FUNCTION__);
    #endif
    return kErrorCode_DataNull;
  }
  // revisar si el free se hace aqui o en la funcion de MEMNODE_free
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

s16 MEMMNODE_free(MemoryNode *node)
{

}

s16 MEMMNODE_softFree(MemoryNode *node)
{

}

s16 MEMMNODE_memSet(MemoryNode *node, u8 value)
{
  if(NULL == node)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] node data NULL\n", __FUNCTION__);
    #endif
    return kErrorCode_DataNull;
  }
  if(0 == node->size_)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] node size = 0, need some value\n", __FUNCTION__);
    #endif
    return kErrorCode_SizeZero;
  }
  memset(node->data_, value, node->size_);
  return kErrorCode_Ok;
}

s16 MEMMNODE_memCopy(MemoryNode *node, void *src, u16 bytes)
{
  if(NULL == node)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_ || NULL == src)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] node data NULL\n", __FUNCTION__);
    #endif
    return kErrorCode_DataNull;
  }
  if(bytes > node->size_)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] not enought node size\n", __FUNCTION__);
    #endif
    return kErrorCode_SizeMismatch;
  }
  memcpy(node->data_, src, bytes);

  return kErrorCode_Ok;

}

s16 MEMMNODE_memConcat(MemoryNode *node, void *src, u16 bytes)
{

}

s16 MEMMNODE_memMask(MemoryNode *node, u8 mask)
{

}

void MEMMNODE_print(MemoryNode *node)
{
  if(NULL == node)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
    #endif
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_)
  {
    #ifdef VERBOSE_
    printf("Error: [%s] data NULL\n", __FUNCTION__);
    #endif
    return kErrorCode_DataNull;

  }
  printf("Memory Node:\n");
  printf("Size: %u bytes\n", node->size_);
  printf("Data:\n");

  unsigned char *data = (unsigned char*)node->data_;
  for(u16 i = 0; i < node->size_; i++)
  {
    printf("%02X ", data[i]);
  }
  printf("\n");
  return kErrorCode_Ok;
}
