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
static s16 LIST_insertFirst(List* list, void* data, u16 size);
static s16 LIST_insertLast(List* list, void* data, u16 size);
static s16 LIST_insertAt(List *list, void* data, u16 size, u16 index);
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

List* LIST_create(u16 capacity)
{
    if(0 >= capacity)
    {
        return NULL;
    }
    List *list_ = MM->malloc(sizeof(List));
    if(NULL == list_)
    {
        return NULL;
    }

    list_->head_ = NULL;
    list_->tail_ = NULL;
    list_->capacity_ = capacity;
    list_->length_ = 0;
    list_->ops_ = &list_ops;
    return list_;

}

MemoryNode* LIST_next(MemoryNode *node)
{
    if(NULL == node)
    {

        return NULL;
    }
    return node->next_;
}

s16 LIST_setNext(MemoryNode *node, MemoryNode *next)
{
    if(NULL == node || NULL == next){
        return kErrorCode_NodeNull;
    }
    node->next_ = next;
    return kErrorCode_Ok;
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
  if (list == NULL)
    {
        return kErrorCode_ListNull;
    }

    // delete all nodes
    MemoryNode *current_node = list->head_;
    MemoryNode *next_node;

    while (current_node != NULL)
    {
        next_node = current_node->next_;
        free(current_node);
        current_node = next_node;
    }
    list->head_ = NULL;
    list->tail_ = NULL;
    list->length_ = 0;
    list->capacity_ = 0;

    return kErrorCode_Ok;

  
}

s16 LIST_reset(List *list)
{
      if (list == NULL)
    {
        return kErrorCode_ListNull;
    }

    // delete all nodes
    MemoryNode *current_node = list->head_;
    MemoryNode *next_node;

    while (current_node != NULL)
    {
        next_node = current_node->next_;
        MM->free(current_node->data_);
        current_node->data_ = NULL;
        MM->free(current_node);
        current_node = next_node;
    }
    list->head_ = NULL;
    list->tail_ = NULL;
    list->length_ = 0;

    return kErrorCode_Ok;
}

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

    if(list->capacity_ > new_capacity)
    {

        MemoryNode *current_node = list->head_;
        MemoryNode *next_node;

        while (current_node != NULL)
        {
            next_node = current_node->next_;

            if (next_node != NULL)
            {
                current_node->next_ = NULL;
                free(next_node->data_);
                free(next_node);
                list->length_--;
                list->capacity_--;
            }

            current_node = current_node->next_;
        }

        list->capacity_ = new_capacity;

    }
        return kErrorCode_Ok;
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
        return False;
    }
    if(list->length_ != 0){
        return False;
    }
    return True;
}

boolean LIST_isFull(List *list)
{
    if(list == NULL)
    {
        return False;
    }
    if(list->length_ != list->capacity_ || list->length_ == 0){
        return False;
    }
    return True;
}

void* LIST_first(List *list)
{
    if(list == NULL)
    {
        return NULL;
    }
    if(list->head_ == NULL)
    {
        return NULL;
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

s16 LIST_insertFirst(List *list, void *data, u16 size)
{

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode *node = MEMNODE_create();
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    node->size_ = size;
    node->data_ = data;
    if(LIST_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    //check if the list is empty
    if (LIST_isEmpty(list))
    {
        node->next_ = NULL;
        list->head_ = node;
        list->tail_ = node;
        list->length_ = 1;
    }
    else
    {
        //check if have enought capacity
        node->next_ = list->head_;
        list->head_ = node;
        list->length_++;
    }
    return kErrorCode_Ok;
}

s16 LIST_insertLast(List* list, void* data, u16 size)
{
    if (list == NULL)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode* node = MEMNODE_create();
    if (node == NULL)
    {
        return kErrorCode_NodeNull;
    }
    node->size_ = size;
    node->data_ = data;
    if (list->length_ >= list->capacity_)
    {
        return kErrorCode_NotEnoughCapacity;
    }

    //check if the list is empty
    if (LIST_isEmpty(list))
    {
        node->next_ = NULL;
        list->head_ = node;
        list->tail_ = node;
        list->length_ = 1;
    }
    else
    {
        // Insert node at end
        list->tail_->next_ = node;
        list->tail_ = node;
        list->length_++;
    }

    return kErrorCode_Ok;
}

s16 LIST_insertAt(List *list, void* data, u16 size, u16 index)
{
    if (list == NULL)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode* node = MEMNODE_create();
    if (node == NULL)
    {
        return kErrorCode_NodeNull;
    }
    node->size_ = size;
    node->data_ = data;

    // Check index
    if (index > list->length_)
    {
        return kErrorCode_InvalidIndex;
    }

    // check if is full
    if (LIST_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }

    // insert first
    if (index == 0)
    {
        return LIST_insertFirst(list, node->data_, node->size_);
    }

    // insert last
    if (index == list->length_)
    {
        return LIST_insertLast(list, node->data_, node->size_);
    }

    // Insert node in index
    MemoryNode *current_node = list->head_;
    for(u16 i = 0; i < index - 1; i++)
    {
        current_node = current_node->next_;
    }
    node->next_ = current_node->next_;
    current_node->next_ = node;
    list->length_++;

    return kErrorCode_Ok;
}

s16 LIST_extractFirst(List *list)
{
    if (list == NULL)
    {
        return kErrorCode_ListNull;
    }

    // check list empty
    if (LIST_isEmpty(list))
    {
        return kErrorCode_ListEmpty;
    }

    MemoryNode *node_to_extract = list->head_;
    list->head_ = list->head_->next_;

    if (list->length_ == 1)
    {
        list->tail_ = NULL;
    }

    free(node_to_extract->data_);
    free(node_to_extract);

    list->length_--;

    return 0;
}

s16 LIST_extractLast(List *list)
{
     if (list == NULL)
    {
        return kErrorCode_ListNull;
    }

    // Verifica si la lista está vacía
    if (LIST_isEmpty(list))
    {
        return kErrorCode_ListEmpty;
    }

    // Check only 1 node
    if (list->length_ == 1)
    {
        MemoryNode *node_to_extract = list->head_;
        list->head_ = NULL;
        list->tail_ = NULL;
        free(node_to_extract->data_);
        free(node_to_extract);
    }
    else
    {
        // search list tail - 1
        MemoryNode *prev_node = NULL;
        MemoryNode *current_node = list->head_;
        while (current_node->next_ != NULL)
        {
            prev_node = current_node;
            current_node = current_node->next_;
        }

        // Extract last and update tail
        list->tail_ = prev_node;
        list->tail_->next_ = NULL;
        free(current_node->data_);
        free(current_node);
    }
    list->length_--;

    return kErrorCode_Ok;
}

s16 LIST_extractAt(List *list, u16 index)
{
    if (list == NULL)
    {
        return kErrorCode_ListNull;
    }

    if (index >= list->length_)
    {
        return kErrorCode_InvalidIndex;
    }

    if (LIST_isEmpty(list))
    {
        return kErrorCode_ListEmpty;
    }

    MemoryNode *node_to_extract;
    
    // If extractFirst
    if (index == 0)
    {
       LIST_extractFirst(list);
       return kErrorCode_Ok;
    }
    else
    {
        // search prev node to extract
        MemoryNode *prev_node = NULL;
        MemoryNode *current_node = list->head_;
        u16 current_index = 0;

        while (current_index < index)
        {
            prev_node = current_node;
            current_node = current_node->next_;
            current_index++;
        }

        node_to_extract = current_node;

        // if extract last
        if (index == list->length_ - 1)
        {
            LIST_extractLast(list);
            return kErrorCode_Ok;
        }
        else
        {
            prev_node->next_ = current_node->next_;
        }
    }

    // Free
    free(node_to_extract->data_);
    free(node_to_extract);

    list->length_--;

    return kErrorCode_Ok;
}

s16 LIST_concat(List *list, List *next_list)
{
    if (list == NULL || next_list == NULL)
    {
        return kErrorCode_ListNull;
    }
    // check if next is empty
    if (LIST_isEmpty(next_list))
    {
        return kErrorCode_Ok;
    }
    // check if list is empty and copy each other
    if (LIST_isEmpty(list))
    {
        list->head_ = next_list->head_;
        list->tail_ = next_list->tail_;
        list->length_ = next_list->length_;
    }
    else
    {
        // Concat list :)
        list->tail_->next_ = next_list->head_;
        list->tail_ = next_list->tail_;
        list->length_ += next_list->length_;
    }

    // freeeeeeee memory uwu
    free(next_list);

    return kErrorCode_Ok;
}

s16 LIST_traverse(List *list, void(*callback)(MemoryNode*))
{
    if (list == NULL)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode *current_node = list->head_;
    while (current_node != NULL)
    {
        // callback current node
        callback(current_node);
        // next node
        current_node = current_node->next_;
    }

    return kErrorCode_Ok;
}

void LIST_print(List *list)
{
    if (list == NULL)
    {
        printf("[List Info] List is NULL\n");
        return;
    }

    printf("[List Info] Address: %p\n", (void *)list);
    printf("[List Info] Length: %u\n", list->length_);
    printf("[List Info] Capacity: %u\n", list->capacity_);

    MemoryNode *current_node = list->head_;
    int storage_index = 0;

    while (current_node != NULL)
    {
        u8* node_ptr_char = (u8*)current_node->data_;
        printf("[List Info] Storage [#%d]:\n", storage_index);
        printf("        [Node Info] Address: %p\n", (void *)current_node);
        printf("        [Node Info] Size: %u\n", current_node->size_);
        printf("        [Node Info] Data address: %p\n", current_node->data_);
        printf("        [Node Info] Data content: ");

        for (int i = 0; i < current_node->size_; i++) {
            printf("%c", node_ptr_char[i]);
        }

        printf("\n      [Node Info] Next address: %p\n", (void *)current_node->next_);

        current_node = current_node->next_;
        storage_index++;
    }
}




