// adt_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "adt_memory_node.h"

typedef struct adt_vector_s {
	u16 head_;
	u16 tail_; // Corresponds to the first free place of the vector
	u16 capacity_;
	MemoryNode *storage_;
	struct vector_ops_s *ops_;
} Vector;

struct vector_ops_s {

  /**
 * @brief Destroys a vector, freeing its memory and memory nodes.
 *
 * This function destroys a vector by freeing the memory occupied by its storage
 * (memory nodes) and the vector structure itself. It also resets each memory node
 * in the storage before freeing it.
 *
 * @param vector Pointer to the vector to be destroyed.
 * @return kErrorCode_Ok on success, kErrorCode_VectorNull if the input vector is NULL.
 */
	s16 (*destroy)(Vector *vector);	// Destroys the vector and its data

  /**
 * @brief Performs a soft reset on a vector, clearing its memory nodes.
 *
 * This function performs a soft reset on the provided vector by calling the
 * softReset function on each memory node in its storage. It also sets the tail
 * index to 0, indicating that no elements are present in the vector.
 *
 * @param vector Pointer to the vector to be soft reset.
 * @return kErrorCode_Ok on success, kErrorCode_VectorNull if the input vector is NULL,
 *         kErrorCode_StorageNull if the storage in the vector is NULL.
 */
  s16 (*softReset)(Vector *vector);		// Soft resets vector's data

  /**
 * @brief Resets a vector, clearing its memory nodes and indices.
 *
 * This function performs a full reset on the provided vector by calling the
 * reset function on each memory node in its storage. It sets both the head and
 * tail indices to 0, indicating that no elements are present in the vector.
 *
 * @param vector Pointer to the vector to be reset.
 * @return kErrorCode_Ok on success, kErrorCode_VectorNull if the input vector is NULL,
 *         kErrorCode_StorageNull if the storage in the vector is NULL.
 */
  s16 (*reset)(Vector *vector);		// Resets vector's data

  /**
 * @brief Resizes the capacity of a vector.
 *
 * This function resizes the capacity of the provided vector to the specified new capacity.
 * It allocates a new storage array with the updated capacity, copies the existing data from
 * the old storage to the new storage, and frees the memory occupied by the old storage.
 * If the new capacity is greater than the current tail index, additional memory nodes are
 * created and initialized.
 *
 * @param vector Pointer to the vector to be resized.
 * @param new_capacity The new capacity for the vector.
 * @return kErrorCode_Ok on success, kErrorCode_VectorNull if the input vector is NULL,
 *         kErrorCode_StorageNull if the storage in the vector is NULL, kErrorCode_SizeZero
 *         if the new capacity is zero, or kErrorCode_VectorFull if memory allocation fails.
 */
	s16 (*resize)(Vector *vector, u16 new_size); // Resizes the capacity of the vector. Some elements can be lost

	/**
 * @brief Returns the capacity of a vector.
 *
 * This function returns the capacity of the provided vector.
 *
 * @param vector Pointer to the vector to query for capacity.
 * @return The capacity of the vector, or 0 if the input vector is NULL.
 */
	u16 (*capacity)(Vector *vector);		// returns the maximum number of elemets to store
	
  /**
 * @brief Returns the length (number of elements) of a vector.
 *
 * This function returns the length of the provided vector, calculated as the difference
 * between the tail index and the head index.
 *
 * @param vector Pointer to the vector to query for length.
 * @return The length of the vector, or 0 if the input vector is NULL.
 */
  u16 (*length)(Vector *vector);			// current number of elements (<= capacity)
	
  /**
 * @brief Checks if a vector is empty.
 *
 * This function checks if the provided vector is empty by comparing the head and tail indices.
 *
 * @param vector Pointer to the vector to check for emptiness.
 * @return True if the vector is empty, False otherwise. Returns True if the input vector is NULL.
 */
  boolean (*isEmpty)(Vector *vector);

  /**
 * @brief Checks if a vector is full.
 *
 * This function checks if the provided vector is full by comparing the tail index with the
 * vector's capacity. If the tail index is greater than or equal to the capacity, the vector is
 * considered full.
 *
 * @param vector Pointer to the vector to check for fullness.
 * @return True if the vector is full, False otherwise. Returns True if the input vector is NULL.
 */
	boolean (*isFull)(Vector *vector);


	/*
   _____        _                                _           
  |  __ \      | |                              (_)          
  | |  | | __ _| |_ __ _    __ _ _   _  ___ _ __ _  ___  ___ 
  | |  | |/ _` | __/ _` |  / _` | | | |/ _ \ '__| |/ _ \/ __|
  | |__| | (_| | || (_| | | (_| | |_| |  __/ |  | |  __/\__ \
  |_____/ \__,_|\__\__,_|  \__, |\__,_|\___|_|  |_|\___||___/
                              | |                            
                              |_|                            
  */
  /**
 * @brief Returns a reference to the first element in a vector.
 *
 * This function returns a reference to the data of the first element in the provided vector.
 * If the vector is empty or if the input vector is NULL, it returns NULL.
 *
 * @param vector Pointer to the vector from which to retrieve the first element.
 * @return A reference to the data of the first element in the vector, or NULL if the vector
 *         is empty or the input vector is NULL.
 */
	void* (*first)(Vector *vector); // Returns a reference to the first element of the vector
	
  /**
 * @brief Returns a reference to the last element in a vector.
 *
 * This function returns a reference to the data of the last element in the provided vector.
 * If the vector is empty or if the input vector is NULL, it returns NULL.
 *
 * @param vector Pointer to the vector from which to retrieve the last element.
 * @return A reference to the data of the last element in the vector, or NULL if the vector
 *         is empty or the input vector is NULL.
 */
  void* (*last)(Vector *vector); // Returns a reference to the last element of the vector
	

  /**
 * @brief Returns a reference to the element at a specified position in a vector.
 *
 * This function returns a reference to the data of the element at the specified position
 * in the provided vector. If the vector is empty, if the input vector is NULL, or if the
 * specified position is beyond the tail index, it returns NULL.
 *
 * @param vector Pointer to the vector from which to retrieve the element.
 * @param position The position of the element to retrieve.
 * @return A reference to the data of the element at the specified position in the vector,
 *         or NULL if the vector is empty, the input vector is NULL, or the position is
 *         beyond the tail index.
 */
  void* (*at)(Vector *vector, u16 position); // Returns a reference to the element oat a given position


	/*  
  _____                     _   _             
 |_   _|                   | | (_)            
   | |  _ __  ___  ___ _ __| |_ _  ___  _ __  
   | | | '_ \/ __|/ _ \ '__| __| |/ _ \| '_ \ 
  _| |_| | | \__ \  __/ |  | |_| | (_) | | | |
 |_____|_| |_|___/\___|_|   \__|_|\___/|_| |_|
                                              
  */
  /**
 * @brief Inserts an element at the beginning of a vector.
 *
 * This function inserts an element with the specified data and size at the beginning
 * of the provided vector. It shifts existing elements to make room for the new element.
 * If the vector is full, if the input vector is NULL, if the storage in the vector is NULL,
 * if the data is NULL, or if the specified size is zero, it returns an error code.
 *
 * @param vector Pointer to the vector in which to insert the element.
 * @param data Pointer to the data of the element to be inserted.
 * @param bytes The size of the data to be inserted.
 * @return kErrorCode_Ok on success, or an appropriate error code if an error occurs.
 */
	s16 (*insertFirst)(Vector *vector, void *data, u16 bytes); // Inserts an element in the first position of the vector
	
  /**
 * @brief Inserts an element at the end of a vector.
 *
 * This function inserts an element with the specified data and size at the end
 * of the provided vector. If the vector is full, if the input vector is NULL, if
 * the storage in the vector is NULL, if the data is NULL, or if the specified size
 * is zero, it returns an error code.
 *
 * @param vector Pointer to the vector in which to insert the element.
 * @param data Pointer to the data of the element to be inserted.
 * @param bytes The size of the data to be inserted.
 * @return kErrorCode_Ok on success, or an appropriate error code if an error occurs.
 */
  s16 (*insertLast)(Vector *vector, void *data, u16 bytes); // Inserts an element in the last position of the vector
	
  
/**
 * @brief Inserts an element at a specified position in a vector.
 *
 * This function inserts an element with the specified data and size at the specified
 * position in the provided vector. If the vector is full, if the input vector is NULL,
 * if the storage in the vector is NULL, if the data is NULL, if the specified size is zero,
 * or if the specified position is beyond the vector's capacity, it returns an appropriate
 * error code. If the specified position is beyond the tail index, the element is inserted
 * at the end of the vector.
 *
 * @param vector Pointer to the vector in which to insert the element.
 * @param data Pointer to the data of the element to be inserted.
 * @param bytes The size of the data to be inserted.
 * @param position The position at which to insert the element.
 * @return kErrorCode_Ok on success, or an appropriate error code if an error occurs.
 */
  s16 (*insertAt)(Vector *vector, void *data, u16 bytes, u16 position); // Inserts an element at the given position of the vector

	/*
  ______      _                  _   _             
 |  ____|    | |                | | (_)            
 | |__  __  _| |_ _ __ __ _  ___| |_ _  ___  _ __  
 |  __| \ \/ / __| '__/ _` |/ __| __| |/ _ \| '_ \ 
 | |____ >  <| |_| | | (_| | (__| |_| | (_) | | | |
 |______/_/\_\\__|_|  \__,_|\___|\__|_|\___/|_| |_|                               
  */
  /**
 * @brief Extracts and returns the first element from a vector.
 *
 * This function extracts and returns the data of the first element from the provided vector.
 * If the vector is empty, if the input vector is NULL, or if the storage in the vector is NULL,
 * it returns an appropriate error code. The extracted element is removed from the vector, and the
 * remaining elements are shifted to fill the gap.
 *
 * @param vector Pointer to the vector from which to extract the first element.
 * @return A pointer to the data of the extracted element, or an appropriate error code if an error occurs.
 */
	void* (*extractFirst)(Vector *vector); // Extracts the first element of the vector
  /*
	// For debugging:
#if DEBUG
	void* (*extractFirst)(Vector *vector, u16 *size);
#endif // DEBUG
	void* (*extractFirst)(Vector *vector
#if DEBUG
	, u16 *size
#endif // DEBUG
		);
  */
 /**
 * @brief Extracts and returns the last element from a vector.
 *
 * This function extracts and returns the data of the last element from the provided vector.
 * If the vector is empty, if the input vector is NULL, or if the storage in the vector is NULL,
 * it returns an appropriate error code. The extracted element is removed from the vector.
 *
 * @param vector Pointer to the vector from which to extract the last element.
 * @return A pointer to the data of the extracted element, or an appropriate error code if an error occurs.
 */
	void* (*extractLast)(Vector *vector); // Extracts the last element of the vector
	
  /**
 * @brief Extracts and returns the element at a specified position in a vector.
 *
 * This function extracts and returns the data of the element at the specified position
 * in the provided vector. If the vector is empty, if the input vector is NULL, if the storage
 * in the vector is NULL, or if the specified position is beyond the vector's tail index,
 * it returns an appropriate error code. The extracted element is removed from the vector, and
 * the remaining elements are shifted to fill the gap.
 *
 * @param vector Pointer to the vector from which to extract the element.
 * @param position The position of the element to be extracted.
 * @return A pointer to the data of the extracted element, or an appropriate error code if an error occurs.
 */
  void* (*extractAt)(Vector *vector, u16 position); // Extracts the element of the vector at the given position

	/*
  __  __ _              _ _                                  
 |  \/  (_)            | | |                                 
 | \  / |_ ___  ___ ___| | | __ _ _ __   ___  ___  _   _ ___ 
 | |\/| | / __|/ __/ _ \ | |/ _` | '_ \ / _ \/ _ \| | | / __|
 | |  | | \__ \ (_|  __/ | | (_| | | | |  __/ (_) | |_| \__ \
 |_|  |_|_|___/\___\___|_|_|\__,_|_| |_|\___|\___/ \__,_|___/
                                                             
  */
  /**
 * @brief Concatenates two vectors by combining their elements into a new vector.
 *
 * This function concatenates two vectors, `vector` and `vector_src`, by creating a new vector
 * that contains all the elements from both vectors. The elements from the `vector` are copied first,
 * followed by the elements from the `vector_src`. The original vectors remain unchanged. If either
 * of the input vectors or their storages is NULL, or if there is not enough memory to create the
 * new vector, it returns an appropriate error code.
 *
 * @param vector Pointer to the destination vector to which elements will be concatenated.
 * @param vector_src Pointer to the source vector from which elements will be concatenated.
 * @return An appropriate error code if an error occurs, or kErrorCode_Ok if the concatenation is successful.
 */
	s16 (*concat)(Vector *vector, Vector *vector_src); // Concatenates two vectors

  /**
 * @brief Traverses the elements of a vector and applies a callback function to each element.
 *
 * This function traverses the elements of the provided vector, starting from the head index
 * up to (but not including) the tail index. For each element, the specified callback function
 * is called, passing a pointer to the `MemoryNode` representing the element as an argument.
 * If the input vector is NULL or if the callback function is NULL, it returns an appropriate error code.
 *
 * @param vector Pointer to the vector to be traversed.
 * @param callback Pointer to the callback function to be applied to each element.
 * @return An appropriate error code if an error occurs, or kErrorCode_Ok if the traversal is successful.
 */
	s16 (*traverse)(Vector *vector, void (*callback)(MemoryNode *)); // Calls to a function from all elements of the vector
	
  /**
 * @brief Prints information about the vector, including its properties and the content of each element.
 *
 * This function prints information about the provided vector, including its address, head index, tail index,
 * length, capacity, and the content of each element in its storage. If the input vector is NULL or if its
 * storage is NULL, it prints minimal information and returns. Otherwise, it iterates through the elements
 * in the vector's storage and prints detailed information about each element.
 *
 * @param vector Pointer to the vector for which information will be printed.
 */
  void (*print)(Vector *vector); // Prints the features and content of the vector
};

/**
 * @brief Creates a new vector with the specified capacity.
 *
 * This function creates a new vector with the specified capacity. It allocates memory
 * for the vector structure and its storage, initializes each element in the storage as
 * a memory node, and sets the vector properties such as head, tail, capacity, and operations.
 *
 * @param capacity The capacity of the vector.
 * @return A pointer to the newly created vector on success, or NULL if there is not
 *         enough memory available or if the specified capacity is not positive.
 */

Vector* VECTOR_create(u16 capacity); // Creates a new vector
#endif //__ADT_VECTOR_H__
