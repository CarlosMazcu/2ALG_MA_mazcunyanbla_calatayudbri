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

static MemoryNode* LIST_next(MemoryNode *node);
static s16 LIST_setNext(MemoryNode *node, MemoryNode *next);
static s16 LIST_destroy(List *list);
static s16 LIST_reset(List *list);
static s16 LIST_softReset(List *List);
static s16 LIST_resize(List *list, u16 new_capacity);
static u16 LIST_capacity(List *list);
static u16 LIST_lenght(List *list);
static boolean LIST_isEmpty(List *list);
static boolean LIST_isFull(List *list);
static void* LIST_first(List *list);
static void* LIST_last(List *list);
static void* LIST_at(List *list);
static s16 LIST_insertFirst(List *list, MemoryNode *node);
static s16 LIST_insertLast(List *list, MemoryNode *node);
static s16 LIST_insertAt(List *list, MemoryNode *node, u16 index);
static s16 LIST_extractFirst(List *list, MemoryNode *node);
static s16 LIST_extractLast(List *list, MemoryNode *node);
static s16 LIST_extractAt(List *list, MemoryNode *node, u16 index);
static s16 LIST_concat(List *list, List *list);
static s16 LIST_traverse(List *list, void (*callback)(MemoryNode *));
static void* LIST_print(List *list);

// List's API Definitions
struct memory_node_ops_s memory_node_ops = { .next = LIST_next
                                             .setNext = LIST_setNext,
                                             .destroy = LIST_destroy,
                                             .reset = LIST_reset,
                                             .softReset = LIST_softReset,
                                             .resize = LIST_resize,
                                             .capacity = LIST_capacity,
                                             .lenght = LIST_lenght,
                                             .isEmpty = LIST_isEmpty,
                                             .isFull = LIST_isFull,
                                             .first = LIST_first,
                                             .last = LIST_last,
                                             .at = LIST_at,
                                             .insertFirst = LIST_insertFirst,
                                             .insertLast = LIST_insertLast,
                                             .insertAt = LIST_insertAt,
                                             .extractFirst = LIST_extractFirst,
                                             .extractLast = LIST_extractLast,
                                             .extractAt = LIST_extractAt,
                                             .concat = LIST_concat,
                                             .traverse = LIST_traverse,
                                             .print = LIST_print,
};

MemoryNode* LIST_next(MemoryNode *node)
{
  
}