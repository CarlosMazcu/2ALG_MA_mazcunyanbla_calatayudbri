/**
 * @file adt_memory_node.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "EDK_MemoryManager/edk_platform_types.h"
#include "adt_memory_node.h"

// Memory Node type
typedef struct list_s {
  MemoryNode* head_;
  MemoryNode* tail_;
  u16 length_;
  u16 capacity_;
  struct list_ops_s *ops_;
} List;

// Memory Node's API Declarations

struct list_ops_s {

  MemoryNode*(*next)(MemoryNode *node);

  s16(*setNext)(MemoryNode *node, MemoryNode *next);

  s16(*destroy)(List *list);

  s16(*softReset)(List *List);

  s16(*reset)(List *list);

  s16(*resize)(List *list, u16 new_capacity);

  u16(*capacity)(List *list);//

  u16(*length)(List *list);//

  boolean(*isEmpty)(List *list);

  boolean(*isFull)(List *list);

  void*(*first)(List *list);

  void*(*last)(List *list);

  void*(*at)(List *list, u16 index);

  s16(*insertFirst)(List *list, MemoryNode *node);

  s16(*insertLast)(List *list, MemoryNode *node);

  s16(*insertAt)(List *list, MemoryNode *node, u16 index);

  s16(*extractFirst)(List *list);

  s16(*extractLast)(List *list);

  s16(*extractAt)(List *list, u16 index);

  s16(*concat)(List *list, List *next_list);

  s16 (*traverse)(List *list, void (*callback)(MemoryNode *));

  void(*print) (List *list);
};


#endif // __ADT_LIST_H__

