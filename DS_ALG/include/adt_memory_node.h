/**
 * @file adt_memory_node.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "EDK_MemoryManager/edk_platform_types.h"

// Memory Node type
typedef struct memory_node_s {
  void *data_;
  u16 size_;
  struct memory_node_s *next_;
  struct memory_node_s *prev_;
  struct memory_node_ops_s *ops_;
} MemoryNode;

// Memory Node's API Declarations

struct memory_node_ops_s {
  /**
 * @brief Retrieves a reference to the data stored in a memory node.
 *
 * This function returns a reference to the 'data_' member of the provided memory node.
 *
 * @param node Pointer to the memory node from which to retrieve the data reference.
 * @return Reference to the data stored in the memory node, or NULL if the input node is NULL.
 *         In case of an error and if VERBOSE_ is defined, an error message is printed.
 */
  void*(*data) (MemoryNode *node);


  /**
 * @brief Sets the data and size for a memory node.
 *
 * This function sets the 'data_' and 'size_' members of the provided memory node
 * with the given source pointer and size in bytes.
 *
 * @param node Pointer to the memory node for which to set the data and size.
 * @param src Pointer to the source data to be assigned to the memory node.
 * @param bytes Size in bytes to be assigned to the memory node.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_SrcNull if the source pointer is NULL, kErrorCode_BytesZero
 *         if the size is zero.
 */
  s16(*setData) (MemoryNode *node, void *src, u16 bytes);


  /**
 * @brief Retrieves the size of the data stored in a memory node.
 *
 * This function returns the size (in bytes) of the data stored in the provided memory node.
 *
 * @param node Pointer to the memory node from which to retrieve the data size.
 * @return Size of the data stored in the memory node, or 0 if the input node is NULL.
 *         In case of an error and if VERBOSE_ is defined, an error message is printed.
 */
  u16(*size) (MemoryNode *node);


/**
 * @brief Resets a memory node, freeing its data and setting size to 0.
 *
 * This function resets a given memory node by freeing its data, setting
 * the data pointer to NULL, and size to 0.
 *
 * @param node Pointer to the memory node to be reset.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_DataNull if the data pointer in the node is NULL.
 */
  s16(*reset) (MemoryNode *node);		// reset content of memory node


  /**
 * @brief Performs a soft reset on a memory node, clearing data and size.
 *
 * This function performs a soft reset on the given memory node by setting
 * the data pointer to NULL and size to 0 without freeing the associated data.
 *
 * @param node Pointer to the memory node to be soft reset.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL.
 */
  s16(*softReset) (MemoryNode *node);		// reset content of memory node without freeing the data
  
  
  /**
 * @brief Frees the memory occupied by a memory node, including its data.
 *
 * This function frees the memory occupied by a memory node, including its data,
 * using the memory manager's free function. It also sets the node's data pointer
 * to NULL and the size to 0.
 *
 * @param node Pointer to the memory node to be freed.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL.
 */
  s16(*free) (MemoryNode *node);		// free memory, both data and node

  /**
 * @brief Performs a soft free on a memory node, freeing only the node itself.
 *
 * This function performs a soft free on the given memory node by freeing only
 * the memory occupied by the node itself, not including the data associated with it.
 *
 * @param node Pointer to the memory node to be soft freed.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL.
 */
  s16(*softFree) (MemoryNode *node);	// free only the node (its mmory, not its data)


/**
 * @brief Sets the memory content of a memory node to a specified value.
 *
 * This function sets the memory content of the data stored in the provided memory node
 * to the specified value using the memset function.
 *
 * @param node Pointer to the memory node for which to set the memory content.
 * @param value The value to set in each byte of the memory content.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_DataNull if the data pointer in the node is NULL,
 *         kErrorCode_SizeZero if the size of the data is zero.
 */
  s16(*memSet) (MemoryNode *node, u8 value);

  /**
 * @brief Copies data from a source to the memory node.
 *
 * This function copies the specified number of bytes from the source pointer to
 * the memory node. If the memory node already contains data, it is freed before
 * copying the new data.
 *
 * @param node Pointer to the memory node to which the data will be copied.
 * @param src Pointer to the source data to be copied.
 * @param bytes Number of bytes to copy from the source to the memory node.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_Null if the source pointer is NULL, kErrorCode_SizeZero if the
 *         specified number of bytes is zero, or kErrorCode_Memory if there is not
 *         enough memory available to allocate space for the copied data.
 */
  s16(*memCopy) (MemoryNode *node, void *src, u16 bytes);


  /**
 * @brief Concatenates data from a source to the memory node.
 *
 * This function concatenates the specified number of bytes from the source pointer
 * to the existing data in the memory node. It allocates new memory to store the
 * concatenated data, frees the existing data, and updates the memory node with the
 * new concatenated data and size.
 *
 * @param node Pointer to the memory node to which the data will be concatenated.
 * @param src Pointer to the source data to be concatenated.
 * @param bytes Number of bytes to concatenate from the source to the memory node.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_DataNull if the data pointer in the node is NULL, kErrorCode_Memory
 *         if there is not enough memory available to allocate space for the concatenated data.
 */
  s16(*memConcat) (MemoryNode *node, void *src, u16 bytes);


  /**
 * @brief Applies a bitmask to the data stored in a memory node.
 *
 * This function applies the specified bitmask to each byte of the data stored in the
 * provided memory node, modifying its contents accordingly.
 *
 * @param node Pointer to the memory node whose data will be masked.
 * @param mask The bitmask to be applied to each byte of the data.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_DataNull if the data pointer in the node is NULL.
 */
  s16(*memMask) (MemoryNode *node, u8 mask);


/**
 * @brief Prints information about a memory node.
 *
 * This function prints information about the provided memory node, including its
 * address, size, data address, and data content.
 *
 * @param node Pointer to the memory node to print information about.
 * @return kErrorCode_Ok on success, kErrorCode_NodeNull if the input node is NULL,
 *         kErrorCode_DataNull if the data pointer in the node is NULL.
 */
  void(*print) (MemoryNode *node);

   /**
 * @brief Sets the next node for a given node.
 *
 * This function sets the 'next_' pointer of the provided node to point to the specified next node.
 *
 * @param node Pointer to the memory node for which to set the next node.
 * @param next Pointer to the next node to be set for the provided node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_NodeNull: Either the provided node or the next node is NULL.
 */
  s16(*setNext)(MemoryNode *node, MemoryNode *next);

     /**
 * @brief Sets the next node for a given node.
 *
 * This function sets the 'next_' pointer of the provided node to point to the specified next node.
 *
 * @param node Pointer to the memory node for which to set the next node.
 * @param next Pointer to the next node to be set for the provided node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_NodeNull: Either the provided node or the next node is NULL.
 */
  s16(*setPrev)(MemoryNode *node, MemoryNode *next);
};

/**
 * @brief Creates a new memory node.
 *
 * This function dynamically allocates memory for a new memory node
 * using a provided memory manager (MM). It then initializes the
 * memory node and returns a pointer to it.
 *
 * @return Pointer to the newly created memory node, or NULL on failure.
 *         On failure, an error message is printed if VERBOSE_ is defined.
 */
MemoryNode* MEMNODE_create();


/**
 * @brief Creates a memory node from a reference pointer.
 *
 * This function creates a new memory node and assigns its address to
 * the reference pointer provided. It also initializes the memory node.
 *
 * @param node Pointer to the reference pointer to be assigned the address of the new memory node.
 * @return kErrorCode_Ok on success, kErrorCode_Memory if the input reference pointer is NULL,
 *         or if there is not enough memory available to create the memory node.
 */
s16 MEMNODE_createFromRef(MemoryNode **node);


/**
 * @brief Creates a lightweight memory node.
 *
 * This function initializes a given memory node with default values,
 * making it a lightweight version. It sets the data pointer to NULL,
 * the size to 0, and assigns the memory node operations structure.
 *
 * @param node Pointer to the memory node to be initialized.
 * @return kErrorCode_Ok on success, kErrorCode_Memory if the input node is NULL.
 */
s16 MEMNODE_createLite(MemoryNode *node); // Creates a memory node without memory allocation

#endif // __ADT_MEMORY_NODE_H__

