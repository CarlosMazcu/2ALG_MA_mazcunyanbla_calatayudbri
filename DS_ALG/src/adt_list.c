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
#include "adt_list.h"

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
static void* LIST_at(List *list, u16 index);
static s16 LIST_insertFirst(List *list, MemoryNode *node);
static s16 LIST_insertLast(List *list, MemoryNode *node);
static s16 LIST_insertAt(List *list, MemoryNode *node, u16 index);
static s16 LIST_extractFirst(List *list);
static s16 LIST_extractLast(List *list);
static s16 LIST_extractAt(List *list, u16 index);
static s16 LIST_concat(List *list, List *other_list);
static s16 LIST_traverse(List *list, void (*callback)(MemoryNode *));
static void LIST_print(List *list);

// List's API Definitions
struct list_ops_s list_ops = { .next = LIST_next,
                                             .setNext = LIST_setNext,
                                             .destroy = LIST_destroy,
                                             .reset = LIST_reset,
                                             .softReset = LIST_softReset,
                                             .resize = LIST_resize,
                                             .capacity = LIST_capacity,
                                             .length = LIST_lenght,
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
    if(NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    return node->next_;
}

s16 LIST_setNext(MemoryNode *node, MemoryNode *next)
{
    node->next_ = next;

    return 0;
}

/*Comprobar si es correcto*/
s16 LIST_destroy(List *list) {
  MemoryNode *aux = list->head_;
  MemoryNode *aux_next = aux->next_;
    while(NULL != aux)
    {
        // aux->ops_->free(aux->data_);
        aux->ops_->free(aux);
        aux = aux_next;
        aux_next = aux->next_;
    }

    MM->free(list->head_);
    MM->free(list->tail_);
    MM->free(list);
    return kErrorCode_Ok;
}

s16 LIST_softReset(List *list)
{
  MemoryNode *aux = list;
  MemoryNode *aux_next = aux->next_;

  
}

s16 LIST_reset(List *list)
{}

s16 LIST_resize(List *list, u16 new_capacity)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    if(list->capacity_ <= new_capacity)
    {
        list->capacity_ = new_capacity;
        return kErrorCode_Ok;
    }
    //TODO falta if new_capacity < capacity
}

u16 LIST_capacity(List *list)
{
    if(list == NULL){
        return 0;
    }
    return list->capacity_;
}

u16 LIST_lenght(List *list)
{
    if(list == NULL)
    {
        return 0;
    }
    return list->length_;
}

boolean LIST_isEmpty(List *list)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    if(list->length_ != 0){
        return 0;
    }
    return 1;
}

boolean LIST_isFull(List *list)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    if(list->length_ != list->capacity_){
        return 0;
    }
    return 1;
}

void* LIST_first(List *list)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    if(list->head_ == NULL)
    {
        return kErrorCode_FirstNull;
    }
    return list->head_->data_;
}

void* LIST_last(List *list)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    if(list->tail_ == NULL)
    {
        return kErrorCode_FirstNull;
    }
    return list->tail_->data_;

}

void* LIST_at(List *list, u16 index)
{
    if(list == NULL)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode *aux = list->head_;
    for(int i = 0; i < index; i++)
    {
        aux = aux->next_;
        if(i >= list->length_)
        {
            return aux;
        }
    }
    return aux;
}

s16 LIST_insertFirst(List *list, MemoryNode *node)
{

    return 0;
}

s16 LIST_insertLast(List *list, MemoryNode *node)
{

    return 0;
}

s16 LIST_insertAt(List *list, MemoryNode *node, u16 index)
{

    return 0;
}

s16 LIST_extractFirst(List *list)
{


    return 0;
}

s16 LIST_extractLast(List *list)
{
    if(NULL == list){return kErrorCode_ListNull;}
    MemoryNode *aux = list->head_;
    do{
        aux = aux->next_;
    }while(NULL == aux->next_);
    //terminar
    return 0;
}

s16 LIST_extractAt(List *list, u16 index)
{

    return 0;
}

s16 LIST_concat(List *list, List *next_list)
{
    return 0;
}

s16 LIST_traverse(List *list, void(*callback)(MemoryNode*))
{

}

void LIST_print(List *list)
{
    
}




