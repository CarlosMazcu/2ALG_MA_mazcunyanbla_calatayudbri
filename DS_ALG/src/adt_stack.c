/**
 * @file adt_vector.c
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

 #include <stdio.h>
 #include <stdlib.h>

#include "adt_stack.h"
#include "common_def.h"

static s16 STACK_destroy(Stack *stack);
static s16 STACK_resize(Stack *stack, u16 new_size);
static s16 STACK_reset(Stack *stack);
static u16 STACK_capacity(Stack *stack);
static u16 STACK_length(Stack *stack);
static boolean STACK_isFull(Stack *stack);
static boolean STACK_isEmpty(Stack *stack);
static s16 STACK_push(Stack *stack, void *data, u16 bytes);
static void *STACK_pop(Stack *stack);
static void *STACK_top(Stack *stack);
static s16 STACK_concat(Stack *stack, Stack *stack_src);
static void STACK_print(Stack *stack);

struct stack_ops_s stack_ops = {
    .destroy = STACK_destroy,
    .resize = STACK_resize,
    .reset = STACK_reset,
    .capacity = STACK_capacity,
    .length = STACK_length,
    .isFull = STACK_isFull,
    .isEmpty = STACK_isEmpty,
    .push = STACK_push,
    .pop = STACK_pop,
    .top = STACK_pop,
    .concat = STACK_concat,
    .print = STACK_print,
};

Stack *STACK_create(u16 capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (NULL == stack)
    {
        return NULL;
    }
    stack->storage_ = VECTOR_create(capacity);
    if (NULL == stack->storage_)
    {
        free(stack);
        return NULL;
    }
    stack->ops_ = &stack_ops;

    return stack;
}

s16 STACK_destroy(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    stack->storage_->ops_->destroy(stack->storage_);
    return kErrorCode_Ok;
}

s16 STACK_resize(Stack* stack, u16 new_size)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    stack->storage_->ops_->resize(stack->storage_, new_size);
    return kErrorCode_Ok;
}

s16 STACK_reset(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    stack->storage_->ops_->reset(stack->storage_);
    return kErrorCode_Ok;
}

u16 STACK_capacity(Stack *stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return 0;
    }
   
    return  stack->storage_->ops_->capacity(stack->storage_);;
}

u16 STACK_length(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return 0;
    }
    
    return stack->storage_->ops_->length(stack->storage_);;
}

boolean STACK_isEmpty(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    return stack->storage_->ops_->isEmpty(stack->storage_);
}

boolean STACK_isFull(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    return stack->storage_->ops_->isFull(stack->storage_);
}

s16 STACK_push(Stack* stack, void* data, u16 bytes)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    stack->storage_->ops_->insertLast(stack->storage_, data, bytes);
    return kErrorCode_Ok;
}

void* STACK_pop(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_Ok;
    }
    return stack->storage_->ops_->extractLast(stack->storage_);
}

void* STACK_top(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_Ok;
    }
    return stack->storage_->ops_->last(stack->storage_);
}

s16 STACK_concat(Stack* stack, Stack* stack_src)
{
    if (NULL == stack || NULL == stack_src)
    {
        return kErrorCode_StackNull;
    }

    

    stack->storage_->ops_->concat(stack->storage_, stack_src->storage_);
    return kErrorCode_Ok;
}

void STACK_print(Stack* stack)
{
    if (NULL == stack || NULL == stack->storage_)
    {
        return kErrorCode_StackNull;
    }
    stack->storage_->ops_->print(stack->storage_);

}