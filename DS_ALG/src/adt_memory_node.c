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
  if (NULL == *node) {
    return kErrorCode_Memory;
  }
  *node = MEMNODE_create();
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
        return kErrorCode_DataNull;
  }
  
  if(NULL == node->ops_) {

    return kErrorCode_OpsNull;
  }

    MM->free(node->data_);
    node->data_ = NULL;
    node->size_ = 0;

    MM->free(node->ops_);
    node->ops_ = NULL;  
  return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode *node)
{

  if (NULL == node)
  {
    return kErrorCode_NodeNull;
  }

  MM->free(node);
  node = NULL;
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

s16 MEMNODE_memCopy(MemoryNode *node, void *src, u16 bytes)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_ || NULL == src)
  {
    return kErrorCode_DataNull;
  }
  if(bytes > 0)
  {
    return kErrorCode_SizeMismatch;
  }
  
  memcpy(node->data_, src, bytes);

  return kErrorCode_Ok;

}

s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_ || NULL == src)
  {
    return kErrorCode_DataNull;
  }
  if(0 == bytes || 0 == node->size_)
  {
    return kErrorCode_SizeMismatch;
  }

  u8* aux_data = MM->malloc(node->size_ + bytes);
  if(NULL == aux_data)
  {
    return kErrorCode_Memory;
  }
  
  //REVISAR ESTA PARTE[...]
  /* for(int i = 0; i < (node->size_); i++){
    aux_data[i] = (u8)(node->data_[i]);
  }
  for(int i = node->size_; i < (node->size_ + bytes); i++){
    aux_data[i] = (u8)(src[i]);
  } */



  return kErrorCode_Ok;
}

s16 MEMNODE_memMask(MemoryNode *node, u8 mask)
{
  // MemoryNode *aux = node;
  //aux_node->data_[i] = node->data_[i] & 0
}

void MEMNODE_print(MemoryNode *node)
{
  if(NULL == node)
  {
    return kErrorCode_NodeNull;
  }
  if(NULL == node->data_)
  {
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
