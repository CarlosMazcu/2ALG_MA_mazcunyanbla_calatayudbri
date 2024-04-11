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

static MemoryNode* LIST_next(MemoryNode* node);
static s16 LIST_setNext(MemoryNode* node, MemoryNode* next);
static s16 LIST_destroy(List* list);
static s16 LIST_reset(List* list);
static s16 LIST_softReset(List* List);
static s16 LIST_resize(List* list, u16 new_capacity);
static u16 LIST_capacity(List* list);
static u16 LIST_lenght(List* list);
static boolean LIST_isEmpty(List* list);
static boolean LIST_isFull(List* list);
static void* LIST_first(List* list);
static void* LIST_last(List* list);
static void* LIST_at(List* list, u16 index);
static s16 LIST_insertFirst(List* list, void* data, u16 size);
static s16 LIST_insertLast(List* list, void* data, u16 size);
static s16 LIST_insertAt(List* list, void* data, u16 size, u16 index);
static void* LIST_extractFirst(List* list);
static void* LIST_extractLast(List* list);
static void* LIST_extractAt(List* list, u16 index);
static s16 LIST_concat(List* list, List* other_list);
static s16 LIST_traverse(List* list, void (*callback)(MemoryNode*));
static void LIST_print(List* list);

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
    if (0 >= capacity)
    {
        return NULL;
    }
    List* list_ = MM->malloc(sizeof(List));
    if (NULL == list_)
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

MemoryNode* LIST_next(MemoryNode* node)
{
    if (NULL == node)
    {

        return NULL;
    }
    return node->next_;
}

s16 LIST_setNext(MemoryNode* node, MemoryNode* next)
{
    if (NULL == node || NULL == next) {
        return kErrorCode_NodeNull;
    }
    node->next_ = next;
    return kErrorCode_Ok;
}

/*Comprobar si es correcto*/
s16 LIST_destroy(List* list) {

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (LIST_isEmpty(list))
    {
        MM->free(list->head_);
        MM->free(list->tail_);
        MM->free(list);

        return kErrorCode_Ok;
    }

    MemoryNode* aux = list->head_;
    MemoryNode* aux_next = aux->next_;
    while (NULL != aux)
    {
        aux->ops_->free(aux->data_);
        aux->ops_->free(aux);
        aux = aux_next;
        if (NULL != aux) {
            aux_next = aux->next_;
        }
    }

    MM->free(list->head_);
    MM->free(list->tail_);
    MM->free(list);

    return kErrorCode_Ok;
}

s16 LIST_softReset(List* list)
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    // delete all nodes
    MemoryNode* current_node = list->head_;
    MemoryNode* next_node = current_node->next_;

    while (NULL != current_node)
    {
        //next_node = current_node->next_;
        MM->free(current_node->data_);
        MM->free(current_node);
        current_node = next_node;
    }
    list->head_ = NULL;
    list->tail_ = NULL;
    list->length_ = 0;

    return kErrorCode_Ok;


}

s16 LIST_reset(List* list)
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (list->ops_->isEmpty(list))
    {
        return kErrorCode_Ok;
    }

    // delete all nodes
    MemoryNode* current_node = list->head_;
    MemoryNode* next_node = current_node->next_;

    while (NULL != current_node)
    {
        //next_node = current_node->next_;
        MM->free(current_node->data_);
        current_node = list->head_;
        MM->free(list->head_);
        list->head_ = current_node->next_;
    }
    list->head_ = NULL;
    list->tail_ = NULL;
    list->length_ = 0;

    return kErrorCode_Ok;
}

s16 LIST_resize(List* list, u16 new_capacity)
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (new_capacity <= 0)
    {
        return kErrorCode_InvalidIndex;
    }

    if (list->length_ == new_capacity)
    {
        return kErrorCode_Ok;
    }
    list->capacity_ = new_capacity;

    if (new_capacity > list->length_)
    {
        return kErrorCode_Ok;
    }
    list->length_ = list->capacity_;
    MemoryNode* current_node = list->head_;

    for (u16 i = 0; i < list->capacity_; i++)
    {
        current_node = current_node->next_;
    }
    list->tail_ = current_node;
    list->tail_->next_ = NULL;
  
    MemoryNode* aux = current_node->next_;
    for (aux; NULL != aux; aux->next_)
    {
        current_node->ops_->free(current_node);
        current_node = aux;
        aux = aux->next_;
    }
    
    return kErrorCode_Ok;
}

u16 LIST_capacity(List* list)
{
    if (NULL == list) {
        return 0;
    }
    return list->capacity_;
}

u16 LIST_lenght(List* list)
{
    if (NULL == list)
    {
        return 0;
    }
    return list->length_;
}

boolean LIST_isEmpty(List* list)
{
    if (NULL == list)
    {
        return True;
    }
    if (list->length_ != 0) {
        return False;
    }
    return True;
}

boolean LIST_isFull(List* list)
{
    if (NULL == list)
    {
        return False;
    }
    if (list->length_ != list->capacity_ || list->length_ == 0) {
        return False;
    }
    return True;
}

void* LIST_first(List* list)
{
    if (NULL == list)
    {
        return NULL;
    }
    if (LIST_isEmpty(list))
    {
        return NULL;
    }
    return list->head_->data_;
}

void* LIST_last(List* list)
{
    if (NULL == list)
    {
        return NULL;
    }
    if (NULL == list->tail_)
    {
        return NULL;
    }
    return list->tail_->data_;

}

void* LIST_at(List* list, u16 index)
{
    if (NULL == list)
    {
        return NULL;
    }
    if (index >= list->length_)
    {
        return list->tail_->data_;
    }
    MemoryNode* aux = list->head_;

    if (NULL == aux)
    {
        return NULL;
    }
    for (int i = 0; i < index; ++i)
    {
        aux = aux->next_;
    }

    if (NULL == aux->data_)
    {
        return NULL;
    }
    return aux->data_;
}

s16 LIST_insertFirst(List* list, void* data, u16 size)
{

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (0 == list->capacity_)
    {
        return kErrorCode_NotEnoughCapacity;
    }

    if (LIST_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    MemoryNode* node = MEMNODE_create();
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    node->ops_->setData(node, data, size);
    // node->size_ = size;
    // node->data_ = data;
     //check if the list is empty
    if (LIST_isEmpty(list))
    {
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
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }
    if (LIST_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    MemoryNode* node = MEMNODE_create();
    if (node == NULL)
    {
        return kErrorCode_NodeNull;
    }
    node->ops_->setData(node, data, size);
    //node->size_ = size;
    //node->data_ = data;
    //check if the list is empty
    if (LIST_isEmpty(list))
    {
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

s16 LIST_insertAt(List* list, void* data, u16 size, u16 index)
{

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }
    if (NULL == data)
    {
        return kErrorCode_DataNull;
    }
    // check if is full

    if (LIST_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    // Check index
    if (index > list->length_)
    {
        return list->ops_->insertLast(list, data, size);
    }
    MemoryNode* node = MEMNODE_create();
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    node->ops_->setData(node, data, size);
    //node->size_ = size;
    //node->data_ = data;

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
    MemoryNode* current_node = list->head_;
    for (u16 i = 0; i < index - 1; i++)
    {
        current_node = current_node->next_;
    }
    node->next_ = current_node->next_;
    current_node->next_ = node;
    list->length_++;

    return kErrorCode_Ok;

}

void* LIST_extractFirst(List* list)
{
    if (NULL == list)
    {
        return NULL;
    }

    // check list empty
    if (LIST_isEmpty(list))
    {
        return NULL;
    }

    MemoryNode* node_to_extract = list->head_;
    list->head_ = list->head_->next_;
    list->length_--;
    return node_to_extract;
}

void* LIST_extractLast(List* list)
{
    if (NULL == list)
    {
        return NULL;
    }

    // Verifica si la lista está vacía
    if (LIST_isEmpty(list))
    {
        return NULL;
    }

    MemoryNode* last_node = list->tail_;
    MemoryNode* aux = list->head_;

    while (aux->next_ != last_node && aux != last_node)
    {
        aux = aux->next_;
    }

    list->tail_ = aux;
    list->tail_->next_ = NULL;
    list->length_--;
    if (LIST_isEmpty(list))
    {
        list->head_ = NULL;
        list->tail_ = NULL;
    }

    return last_node->data_;
}


void* LIST_extractAt(List* list, u16 index)
{
    /*
    if (NULL == list)
    {
        return NULL;
    }

    if (index >= list->length_)
    {
        return list->ops_->extractLast(list);
    }

    if (LIST_isEmpty(list))
    {
        return NULL;
    }

    MemoryNode *node_to_extract;

    // If extractFirst
    if (index == 0)
    {
       return LIST_extractFirst(list);
    }
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
    //prev_node->next_ = current_node->next_;
    // Free
    void* extracted_data = node_to_extract->data_;
    MM->free(node_to_extract);
    current_node->next_ = NULL;
    list->length_--;

    return extracted_data;
    */
    if (NULL == list)
    {
        return NULL;
    }
    if (list->ops_->isEmpty(list))
    {
        return NULL;
    }
    if (index >= list->length_)
    {
        return NULL;
    }
    MemoryNode* node;
    MemoryNode* aux = list->head_;
    for (u16 i = 0; i < index - 1; i++)
    {
        aux = aux->next_;
    }
    node = aux->next_;
    aux->next_ = aux->next_->next_;
    list->length_--;
    return node->data_;
}


s16 LIST_concat(List* list, List* next_list)
{
    if (list == NULL || next_list == NULL)
    {
        return kErrorCode_ListNull;
    }

    if (LIST_isEmpty(next_list))
    {
        return kErrorCode_Ok;
    }

    if (LIST_isEmpty(list))
    {
        MemoryNode* current_node = next_list->head_;
        while (current_node != NULL)
        {
            MemoryNode* new_node = MEMNODE_create();
            new_node->size_ = current_node->size_;
            new_node->data_ = current_node->data_;
            if (list->head_ == NULL)
            {
                list->head_ = new_node;
                list->tail_ = new_node;
            }
            else
            {
                list->tail_->next_ = new_node;
                list->tail_ = new_node;
            }

            current_node = current_node->next_;
        }

        list->length_ += next_list->length_;
    }
    else
    {
        list->tail_->next_ = next_list->head_;
        list->tail_ = next_list->tail_;
        list->length_ += next_list->length_;
    }
    return kErrorCode_Ok;
}

s16 LIST_traverse(List* list, void(*callback)(MemoryNode*))
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }
    MemoryNode* current_node = list->head_;
    while (current_node != NULL)
    {
        // callback current node
        callback(current_node);
        // next node
        current_node = current_node->next_;
    }

    return kErrorCode_Ok;
}


void LIST_print(List* list)
{
    if (NULL == list)
    {
        printf("\t[List Info] Address: NULL\n");
        return;
    }
    if (LIST_isEmpty(list))
    {
        printf("\t[List Info] Address: %p\n", list);
        printf("\t[List Info] Head: NULL\n");
        printf("\t[List Info] Tail: NULL\n");
        printf("\t[List Info] Length: %d\n", LIST_lenght(list));
        printf("\t[List Info] Capacity: %d\n", LIST_capacity(list));
        return;
    }
    printf("\t[List Info] Head: %p\n", list->head_);
    printf("\t[List Info] Tail: %p\n", list->tail_);
    printf("\t[List Info] Length: %d\n", LIST_lenght(list));
    printf("\t[List Info] Capacity: %d\n", LIST_capacity(list));
    MemoryNode* aux = list->head_;
    for (u16 i = 0; i < list->ops_->length(list); i++)
    {
        printf("\t\t[List Info] Node #%d\n", i);
        printf("\t\t\t[Node Info] Address: %p\n", aux);
        printf("\t\t\t[Node Info] Size: %d\n", aux->size_);
        printf("\t\t\t[Node Info] Data Address: %p\n", &aux->data_);
        printf("\t\t\t[Node Info] Data Content: ");
        u8* data_byte = aux->data_;
        for (u16 j = 0; j < aux->size_; j++)
        {
            printf("%c", data_byte[j]);
        }
        printf("\n");
        if (NULL == aux->next_)
        {
            printf("\t\t\t[Node Info] Next Adress: NULL\n");
        }
        else
        {
            printf("\t\t\t[Node Info] Next Adress: %p\n", aux->next_);
        }

        aux = aux->next_;
    }
}

