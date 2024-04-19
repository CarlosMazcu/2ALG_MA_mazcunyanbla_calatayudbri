/**
 * @file adt_stack.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2024-04-19
 * @version 1.0
 */

 
#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "EDK_MemoryManager/edk_platform_types.h"
#include "adt_vector.h"

// stack typer
typedef struct stack_s
{
    Vector *storage_;
    struct stack_ops_s *ops_;
} Stack;

// Stack's API declarations

struct stack_ops_s
{
    /**
 * @brief Function pointer type for destroying a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for destroying a stack.
 * The function takes a pointer to a Stack structure as an argument and returns an s16 error code.
 *
 * @param stack Pointer to the stack to be destroyed.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the stack was successfully destroyed.
 *         - Any other error code specific to the destruction process.
 */
    s16 (*destroy)(Stack *stack);

/**
 * @brief Function pointer type for resetting a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for resetting a stack.
 * The function takes a pointer to a Stack structure as an argument and returns an s16 error code.
 *
 * @param stack Pointer to the stack to be reset.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the stack was successfully reset.
 *         - Any other error code specific to the reset process.
 */
    s16 (*reset)(Stack *stack);

    /**
 * @brief Function pointer type for resizing a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for resizing a stack.
 * The function takes a pointer to a Stack structure and the new size as arguments, and returns an s16 error code.
 *
 * @param stack Pointer to the stack to be resized.
 * @param new_size The new size to which the stack will be resized.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the stack was successfully resized.
 *         - Any other error code specific to the resizing process.
 */
    s16 (*resize)(Stack *stack, u16 new_size);

    /**
 * @brief Function pointer type for retrieving the capacity of a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for retrieving the capacity of a stack.
 * The function takes a pointer to a Stack structure as an argument and returns the capacity of the stack as a u16 value.
 *
 * @param stack Pointer to the stack.
 * @return Returns the capacity of the stack.
 */
    u16 (*capacity)(Stack *stack);

    /**
 * @brief Function pointer type for retrieving the length of a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for retrieving the length of a stack.
 * The function takes a pointer to a Stack structure as an argument and returns the length of the stack as a u16 value.
 *
 * @param stack Pointer to the stack.
 * @return Returns the length of the stack.
 */
    u16 (*length)(Stack *stack);

    /**
 * @brief Function pointer type for checking if a stack is empty.
 *
 * This function pointer type defines the signature for a function that is responsible for checking if a stack is empty.
 * The function takes a pointer to a Stack structure as an argument and returns a boolean value indicating whether the stack is empty.
 *
 * @param stack Pointer to the stack.
 * @return Returns `true` if the stack is empty, `false` otherwise.
 */
    boolean (*isEmpty)(Stack *stack);

    /**
 * @brief Function pointer type for checking if a stack is full.
 *
 * This function pointer type defines the signature for a function that is responsible for checking if a stack is full.
 * The function takes a pointer to a Stack structure as an argument and returns a boolean value indicating whether the stack is full.
 *
 * @param stack Pointer to the stack.
 * @return Returns `true` if the stack is full, `false` otherwise.
 */
    boolean (*isFull)(Stack *stack);

    /**
 * @brief Function pointer type for pushing data onto a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for pushing data onto a stack.
 * The function takes a pointer to a Stack structure, a pointer to the data to be pushed, and the size of the data in bytes as arguments.
 * It returns an s16 error code indicating the result of the operation.
 *
 * @param stack Pointer to the stack.
 * @param data Pointer to the data to be pushed onto the stack.
 * @param bytes Size of the data in bytes.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the data was successfully pushed onto the stack.
 *         - Any other error code specific to the push operation.
 */
    s16 (*push)(Stack *stack, void *data, u16 bytes);

    /**
 * @brief Function pointer type for popping data from a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for popping data from a stack.
 * The function takes a pointer to a Stack structure as an argument and returns a pointer to the data popped from the stack.
 * If the stack is empty, it returns NULL.
 *
 * @param stack Pointer to the stack.
 * @return Returns a pointer to the data popped from the stack, or NULL if the stack is empty.
 */
    void *(*pop)(Stack *stack);

    /**
 * @brief Function pointer type for accessing the top element of a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for accessing the top element of a stack.
 * The function takes a pointer to a Stack structure as an argument and returns a pointer to the top element of the stack.
 * If the stack is empty, it returns NULL.
 *
 * @param stack Pointer to the stack.
 * @return Returns a pointer to the top element of the stack, or NULL if the stack is empty.
 */
    void *(*top)(Stack *stack);

    /**
 * @brief Function pointer type for concatenating two stacks.
 *
 * This function pointer type defines the signature for a function that is responsible for concatenating two stacks.
 * The function takes pointers to two Stack structures as arguments and concatenates the source stack onto the destination stack.
 * It returns an s16 error code indicating the result of the operation.
 *
 * @param stack Pointer to the destination stack where the source stack will be concatenated.
 * @param stack_src Pointer to the source stack that will be concatenated onto the destination stack.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the concatenation was successful.
 *         - Any other error code specific to the concatenation operation.
 */
    s16 (*concat)(Stack *stack, Stack *stack_src);

    /**
 * @brief Function pointer type for printing the contents of a stack.
 *
 * This function pointer type defines the signature for a function that is responsible for printing the contents of a stack.
 * The function takes a pointer to a Stack structure as an argument and prints the contents of the stack.
 *
 * @param stack Pointer to the stack to be printed.
 */
    void (*print)(Stack *stack);  
};

/**
 * @brief Create a new stack with the specified capacity.
 *
 * This function creates a new stack with the specified capacity.
 *
 * @param capacity The maximum number of elements the stack can hold.
 * @return Returns a pointer to the newly created stack if successful, or NULL if memory allocation fails.
 */
Stack *STACK_create(u16 capacity);

#endif // __ADT_STACK_H__