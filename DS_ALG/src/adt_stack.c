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

s16 STACK_destroy(Stack *stack);
s16 STACK_resize(Stack *stack);
s16 STACK_reset(Stack *stack);
u16 STACK_capacity(Stack *stack);
u16 STACK_length(Stack *stack);
boolean STACK_isFull(Stack *stack);
boolean STACK_isEmpty(Stack *stack);
s16 STACK_push(Stack *stack, void *data, u16 bytes);
void *STACK_pop(Stack *stack);
void *STACK_top(Stack *stack);
s16 STACK_concat(Stack *stack, Stack *stack_src);
s16 STACK_print(Stack *stack);

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

}