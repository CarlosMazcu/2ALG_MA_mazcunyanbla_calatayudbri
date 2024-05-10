/**
 * @file adt_dllink.c
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_memory_node.h"
#include "adt_dllist.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

static MemoryNode* DLList_next(MemoryNode* node);
static s16 DLList_destroy(DLList* list);
static s16 DLList_reset(DLList* list);
static s16 DLList_softReset(DLList* DLList);
static s16 DLList_resize(DLList* list, u16 new_capacity);
static u16 DLList_capacity(DLList* list);
static u16 DLList_lenght(DLList* list);
static boolean DLList_isEmpty(DLList* list);
static boolean DLList_isFull(DLList* list);
static void* DLList_first(DLList* list);
static void* DLList_last(DLList* list);
static void* DLList_at(DLList* list, u16 index);
static s16 DLList_insertFirst(DLList* list, void* data, u16 size);
static s16 DLList_insertLast(DLList* list, void* data, u16 size);
static s16 DLList_insertAt(DLList* list, void* data, u16 size, u16 index);
static void* DLList_extractFirst(DLList* list);
static void* DLList_extractLast(DLList* list);
static void* DLList_extractAt(DLList* list, u16 index);
static s16 DLList_concat(DLList* list, DLList* other_list);
static s16 DLList_traverse(DLList* list, void (*callback)(MemoryNode*));
static void DLList_print(DLList* list);

// DLList's API Definitions
struct dllist_ops_s dllist_ops = { .next = DLList_next,
                                             .destroy = DLList_destroy,
                                             .reset = DLList_reset,
                                             .softReset = DLList_softReset,
                                             .resize = DLList_resize,
                                             .capacity = DLList_capacity,
                                             .length = DLList_lenght,
                                             .isEmpty = DLList_isEmpty,
                                             .isFull = DLList_isFull,
                                             .first = DLList_first,
                                             .last = DLList_last,
                                             .at = DLList_at,
                                             .insertFirst = DLList_insertFirst,
                                             .insertLast = DLList_insertLast,
                                             .insertAt = DLList_insertAt,
                                             .extractFirst = DLList_extractFirst,
                                             .extractLast = DLList_extractLast,
                                             .extractAt = DLList_extractAt,
                                             .concat = DLList_concat,
                                             .traverse = DLList_traverse,
                                             .print = DLList_print,
};

DLList* DLList_create(u16 capacity)
{
    if (0 >= capacity)
    {
        return NULL;
    }
    DLList* list_ = MM->malloc(sizeof(DLList));
    if (NULL == list_)
    {
        return NULL;
    }

    list_->head_ = NULL;
    list_->tail_ = NULL;
    list_->capacity_ = capacity;
    list_->length_ = 0;
    list_->ops_ = &dllist_ops;
    return list_;

}

MemoryNode* DLList_next(MemoryNode* node)
{
    if (NULL == node)
    {

        return NULL;
    }
    return node->next_;
}

/* s16 DLList_setNext(MemoryNode* node, MemoryNode* next)
{
    if (NULL == node || NULL == next) {
        return kErrorCode_NodeNull;
    }
    node->next_ = next;
    return kErrorCode_Ok;
} */

/*Comprobar si es correcto*/
s16 DLList_destroy(DLList* list) {

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (DLList_isEmpty(list))
    {
        //MM->free(list->head_);
        //MM->free(list->tail_);
        MM->free(list);

        return kErrorCode_Ok;
    }

    MemoryNode* aux = list->head_;
    MemoryNode* aux_next = aux->next_;
    while (NULL != aux)
    {
        //aux->ops_->free(aux->data_);
       // aux->ops_->free(aux);
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

s16 DLList_softReset(DLList* list)
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

s16 DLList_reset(DLList* list)
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    // delete all nodes
    MemoryNode *current_node = list->head_;

    while (NULL != current_node)
    {
        MM->free(list->head_->data_);
        current_node = list->head_->next_;
        MM->free(list->head_);
        list->head_ = current_node;
    }
    list->head_ = NULL;
    list->tail_ = NULL;
    list->length_ = 0;

    return kErrorCode_Ok;
}

s16 DLList_resize(DLList* list, u16 new_capacity)
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

u16 DLList_capacity(DLList* list)
{
    if (NULL == list) {
        return 0;
    }
    return list->capacity_;
}

u16 DLList_lenght(DLList* list)
{
    if (NULL == list)
    {
        return 0;
    }
    return list->length_;
}

boolean DLList_isEmpty(DLList* list)
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

boolean DLList_isFull(DLList* list)
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

void* DLList_first(DLList* list)
{
    if (NULL == list)
    {
        return NULL;
    }
    if (DLList_isEmpty(list))
    {
        return NULL;
    }
    return list->head_->data_;
}

void* DLList_last(DLList* list)
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
//done
void* DLList_at(DLList* list, u16 index)
{
    if (NULL == list)
    {
        return NULL;
    }
    MemoryNode* aux;
    u16 mid = list->length_/2;
    if(index <= mid){
        //empezamos por el principio
        aux = list->head_;
        for (u16 i = 0; i < index && NULL != aux; i++)
        {
            aux = aux->next_;
        }
    }else{
        aux = list->tail_;
        for (u16 i = list->length_ - 1; i > index && NULL != aux; i--)
        {
            aux = aux->prev_;
        }


    }
    if (NULL == aux)
    {
        return NULL;
    }
    if (NULL == aux->data_)
    {
        return NULL;
    }
    return aux->data_;
}

s16 DLList_insertFirst(DLList* list, void* data, u16 size)
{

    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (0 == list->capacity_)
    {
        return kErrorCode_NotEnoughCapacity;
    }

    if (DLList_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    MemoryNode* node = MEMNODE_create();
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    node->ops_->setData(node, data, size);
     //check if the list is empty
    if (DLList_isEmpty(list))
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

s16 DLList_insertLast(DLList* list, void* data, u16 size)
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }
    if (DLList_isFull(list))
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
    if (DLList_isEmpty(list))
    {
        list->head_ = node;
        list->tail_ = node;
        list->length_ = 1;
    }
    else
    {
        // Insert node at end
        list->tail_->next_ = node;
        node->prev_ = list->tail_;
        // TODO: node prev = list tail
        list->tail_ = node;
        list->length_++;
    }

    return kErrorCode_Ok;
}

s16 DLList_insertAt(DLList* list, void* data, u16 size, u16 index)
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
    if (DLList_isFull(list))
    {
        return kErrorCode_NotEnoughCapacity;
    }
    // Check index
    if (index > list->length_)
    {
        return DLList_insertLast(list, data, size);
    }
    MemoryNode* node = MEMNODE_create();
    if (NULL == node)
    {
        return kErrorCode_NodeNull;
    }
    node->ops_->setData(node, data, size);
    node->next_ = NULL;
    node->prev_ = NULL;
    // insert first
    if (index == 0)
    {
        return DLList_insertFirst(list, node->data_, node->size_);
    }
    // insert last
    if (index >= list->length_)
    {
        return DLList_insertLast(list, node->data_, node->size_);
    }
    u16 mid = list->length_/2;
    MemoryNode* current_node = MEMNODE_create();
    if(NULL == current_node){
        return kErrorCode_Null;
    }
    if(index <= mid){
        // Insert node in index
        current_node = list->head_;
        for (u16 i = 0; i < index - 1; i++)
        {
            current_node = current_node->next_;
        }
    }else{
        current_node = list->tail_;
      
        for (u16 i = list->length_ - 1; i > index; i--)
        {
            current_node = current_node->prev_;
        }
    }
    

    node->next_ = current_node->next_;
    node->prev_ = current_node;
    current_node->next_->prev_ = node;
    current_node->next_ = node;
    
    list->length_++;

    return kErrorCode_Ok;

}

void* DLList_extractFirst(DLList* list)
{
    if (NULL == list)
    {
        return NULL;
    }

    // check list empty
    if (DLList_isEmpty(list))
    {
        return NULL;
    }

    MemoryNode* node_to_extract = list->head_;
    list->head_ = list->head_->next_;
    list->length_--;
    return node_to_extract->data_;
}

void* DLList_extractLast(DLList* list)
{
    if (NULL == list)
    {
        return NULL;
    }

    // Verifica si la lista está vacía
    if (DLList_isEmpty(list))
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
    if (DLList_isEmpty(list))
    {
        list->head_ = NULL;
        list->tail_ = NULL;
    }

    return last_node->data_;
}


void* DLList_extractAt(DLList* list, u16 index)
{
    if (NULL == list)
    {
        return NULL;
    }
    if (DLList_isEmpty(list))
    {
        return NULL;
    }
    if (NULL == list->head_) {

        return NULL;
    }

    if (index >= list->length_)
    {
        index = list->length_;
    }
    MemoryNode* node = MEMNODE_create();
    MemoryNode* aux = MEMNODE_create();
    if(NULL == aux || NULL == node)
    {
        return kErrorCode_Null;
    }
    if(0 == index)
    {
        return DLList_extractLast(list);
    }
    if(index >= list->length_)
    {
        return DLList_extractLast(list);
    }
    u16 mid = list->length_/2;
    if(index <= mid){
        aux = list->head_;
        for (u16 i = 0; i < index - 1; i++)
        {
            aux = aux->next_;
        }
    }else{
        aux = list->tail_;
        for(u16 i = list->length_ - 1; i > index; i--)
        {
            aux = aux->prev_;
        }
    }
    node = aux->next_;
    aux->next_ = aux->next_->next_;
    
    list->length_--;
    return node->data_;
}


s16 DLList_concat(DLList* list, DLList* next_list)
{
    if (list == NULL || next_list == NULL)
    {
        return kErrorCode_ListNull;
    }

    if (DLList_isEmpty(next_list))
    {
        return kErrorCode_Ok;
    }

    MemoryNode* current_list = next_list->head_;

    while (NULL != current_list) {

        MemoryNode* new_node = MM->malloc(sizeof(MemoryNode));
        if (new_node == NULL) {
            MM->free(new_node);
            return kErrorCode_StorageNull;
        }

        u8* tmp = MM->malloc(current_list->size_);
        if (NULL == tmp) {
            return kErrorCode_Null;
        }

        u8* aux = (u8*)current_list->data_;
        for (s32 i = 0; i < current_list->size_; i++) {
            tmp[i] = aux[i];
        }

        new_node->data_ = tmp;
        new_node->size_ = current_list->size_;
        new_node->next_ = NULL;

        if (NULL == list->head_) {
            list->head_ = new_node;
            list->tail_ = new_node;
        }
        else {
            
            list->tail_->next_ = new_node;
            list->tail_ = new_node;
        }

        current_list = current_list->next_;
    }

    list->length_ += next_list->length_;
    list->capacity_ += next_list->capacity_;

    return kErrorCode_Ok;
}

s16 DLList_traverse(DLList* list, void(*callback)(MemoryNode*))
{
    if (NULL == list)
    {
        return kErrorCode_ListNull;
    }

    if (NULL == list->head_) {
        return kErrorCode_StorageNull;
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


void DLList_print(DLList* list)
{
    if (NULL == list)
    {
        printf("\t[DLList Info] Address: NULL\n");
        return;
    }
    if (DLList_isEmpty(list))
    {
        printf("\t[DLList Info] Address: %p\n", list);
        printf("\t[DLList Info] Head: NULL\n");
        printf("\t[DLList Info] Tail: NULL\n");
        printf("\t[DLList Info] Length: %d\n", DLList_lenght(list));
        printf("\t[DLList Info] Capacity: %d\n", DLList_capacity(list));
        return;
    }
    printf("\t[DLList Info] Head: %p\n", list->head_);
    printf("\t[DLList Info] Tail: %p\n", list->tail_);
    printf("\t[DLList Info] Length: %d\n", DLList_lenght(list));
    printf("\t[DLList Info] Capacity: %d\n", DLList_capacity(list));
    MemoryNode* aux = list->head_;
    for (u16 i = 0; i < list->ops_->length(list); i++)
    {
        printf("\t\t[DLList Info] Node #%d\n", i);
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

