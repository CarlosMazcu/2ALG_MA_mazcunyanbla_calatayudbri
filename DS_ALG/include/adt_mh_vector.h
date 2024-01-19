/**
 * @file adt_vector.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "adt_memory_node.h"

typedef struct adt_mh_vector_s
{
    u16 head_;
    u16 tail_; // Corresponds to the first free place of the vector
    u16 capacity_;
    MemoryNode *storage_;
    struct mh_vector_ops_s *ops_;
} Mh_Vector;

struct mh_vector_ops_s
{

    s16 (*destroy)(Mh_Vector *vector); // Destroys the vector and its data

   
    s16 (*softReset)(Mh_Vector *vector); // Soft resets vector's data

    
    s16 (*reset)(Mh_Vector *vector); // Resets vector's data

    
    s16 (*resize)(Mh_Vector *vector, u16 new_size); // Resizes the capacity of the vector. Some elements can be lost

    
    u16 (*capacity)(Mh_Vector *vector); // returns the maximum number of elemets to store

   
    u16 (*length)(Mh_Vector *vector); // current number of elements (<= capacity)

    
    boolean (*isEmpty)(Mh_Vector *vector);

    
    boolean (*isFull)(Mh_Vector *vector);
    
    void *(*first)(Mh_Vector *vector); // Returns a reference to the first element of the vector

    
    void *(*last)(Mh_Vector *vector); // Returns a reference to the last element of the vector

   
    void *(*at)(Mh_Vector *vector, u16 position); // Returns a reference to the element oat a given position
   
    s16 (*insertFirst)(Mh_Vector *vector, void *data, u16 bytes); // Inserts an element in the first position of the vector

    
    s16 (*insertLast)(Mh_Vector *vector, void *data, u16 bytes); // Inserts an element in the last position of the vector

    
    s16 (*insertAt)(Mh_Vector *vector, void *data, u16 bytes, u16 position); // Inserts an element at the given position of the vector
    
    void *(*extractFirst)(Mh_Vector *vector); // Extracts the first element of the vector
                                          
    void *(*extractLast)(Mh_Vector *vector); // Extracts the last element of the vector

    void *(*extractAt)(Mh_Vector *vector, u16 position); // Extracts the element of the vector at the given position

    s16 (*concat)(Mh_Vector *vector, Mh_Vector *vector_src); // Concatenates two vectors


    s16 (*traverse)(Mh_Vector *vector, void (*callback)(MemoryNode *)); // Calls to a function from all elements of the vector

  
    void (*print)(Mh_Vector *vector); // Prints the features and content of the vector
};



Mh_Vector *Mh_VECTOR_create(u16 capacity); // Creates a new vector
#endif                               //__ADT_VECTOR_H__
