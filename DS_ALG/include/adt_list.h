/**
 * @file adt_list.h
 * @brief
 * @author <mazcunyanbla@esat-alumni.com> <calatayudbri@esat-alumni.com>
 * @date 2023-11-28
 * @version 1.0
 */

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "EDK_MemoryManager/edk_platform_types.h"
#include "adt_memory_node.h"

// Memory Node type
typedef struct list_s {
  MemoryNode* head_;
  MemoryNode* tail_;
  u16 length_;
  u16 capacity_;
  struct list_ops_s *ops_;
} List;

// Memory Node's API Declarations

struct list_ops_s {
 /**
 * @brief Returns the next node in the list.
 *
 * This function returns a pointer to the next node in the list, following the provided node.
 *
 * @param node Pointer to the memory node from which to obtain the next node.
 * @return Pointer to the next node in the list, or NULL if the input node is NULL.
 *         In case of an error  an error message is printed.
 */
  MemoryNode*(*next)(MemoryNode *node);

/**
 * @brief Destroy a list and free the allocated memory.
 *
 * This function destroys the specified list and frees the memory associated with it.
 * If the list is not empty, it iterates through all nodes and frees the memory allocated for each node and its data.
 *
 * @param list Pointer to the list to be destroyed.
 * @return Returns an error code indicating the result of the operation.
 *         - kErrorCode_Ok if the list was successfully destroyed.
 *         - kErrorCode_ListNull if the `list` pointer is NULL.
 */

  s16(*destroy)(List *list);

  /**
 * @brief Soft reset for a list, clearing all nodes and resetting list properties.
 *
 * This function performs a soft reset for the specified list, clearing all nodes and resetting the list properties.
 * It frees the memory associated with each node and its data but does not free the memory of the list itself.
 *
 * @param list Pointer to the list to be reset.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the soft reset was successful.
 *         - `kErrorCode_ListNull` if the `list` pointer is NULL.
 */

  s16(*softReset)(List *List);

/**
 * @brief Reset a list, clearing all nodes and resetting list properties.
 *
 * This function resets the specified list, clearing all nodes and resetting the list properties.
 * It frees the memory associated with each node and its data, as well as the memory allocated for the list itself.
 *
 * @param list Pointer to the list to be reset.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the reset was successful.
 *         - `kErrorCode_ListNull` if the `list` pointer is NULL.
 */

  s16(*reset)(List *list);

  /**
 * @brief Resizes the capacity of a list.
 *
 * This function resizes the capacity of the provided list to the specified new capacity.
 * If the new capacity is greater than the current capacity, the function updates the capacity of the list.
 * If the new capacity is less than the current capacity, the function removes nodes from the end of the list
 * until the capacity matches the new capacity, freeing memory associated with removed nodes.
 *
 * @param list Pointer to the list to resize.
 * @param new_capacity The new capacity to set for the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 */
  s16(*resize)(List *list, u16 new_capacity);

 /**
 * @brief Retrieves the capacity of a list.
 *
 * This function returns the current capacity of the provided list.
 *
 * @param list Pointer to the list from which to retrieve the capacity.
 * @return The capacity of the list, or 0 if the provided list pointer is NULL.
 */
  u16(*capacity)(List *list);//

 /**
 * @brief Retrieves the length of a list.
 *
 * This function returns the current length of the provided list.
 *
 * @param list Pointer to the list from which to retrieve the length.
 * @return The length of the list, or 0 if the provided list pointer is NULL.
 */
  u16(*length)(List *list);//


 /**
 * @brief Checks if a list is empty.
 *
 * This function checks whether the provided list is empty or not.
 *
 * @param list Pointer to the list to check.
 * @return True if the list is empty, False otherwise. If the provided list pointer is NULL, False is returned.
 */
  boolean(*isEmpty)(List *list);

 /**
 * @brief Checks if a list is full.
 *
 * This function checks whether the provided list is full or not.
 *
 * @param list Pointer to the list to check.
 * @return True if the list is full, False otherwise. If the provided list pointer is NULL, False is returned.
 */
  boolean(*isFull)(List *list);

/**
 * @brief Retrieves the data stored in the first node of a list.
 *
 * This function returns a pointer to the data stored in the first node of the provided list.
 *
 * @param list Pointer to the list from which to retrieve the data from the first node.
 * @return Pointer to the data stored in the first node of the list. 
 *         If the provided list pointer is NULL, kErrorCode_ListNull is returned.
 *         If the first node of the list is NULL, kErrorCode_FirstNull is returned.
 */
  void*(*first)(List *list);

 /**
 * @brief Retrieves the data stored in the last node of a list.
 *
 * This function returns a pointer to the data stored in the last node of the provided list.
 *
 * @param list Pointer to the list from which to retrieve the data from the last node.
 * @return Pointer to the data stored in the last node of the list. 
 *         If the provided list pointer is NULL, kErrorCode_ListNull is returned.
 *         If the last node of the list is NULL, kErrorCode_LastNull is returned.
 */
  void*(*last)(List *list);

 /**
 * @brief Retrieves the data stored in the node at a specified index in a list.
 *
 * This function returns a pointer to the data stored in the node at the specified index in the provided list.
 *
 * @param list Pointer to the list from which to retrieve the data from the node at the specified index.
 * @param index Index of the node from which to retrieve the data.
 * @return Pointer to the data stored in the node at the specified index in the list. 
 *         If the provided list pointer is NULL, kErrorCode_ListNull is returned.
 *         If the index is out of bounds, the pointer to the node at that index is returned.
 */
  void*(*at)(List *list, u16 index);

 /**
 * @brief Inserts a node at the beginning of a list.
 *
 * This function inserts the provided node at the beginning of the provided list.
 *
 * @param list Pointer to the list in which to insert the node.
 * @param node Pointer to the node to insert at the beginning of the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_NotEnoughCapacity: The list does not have enough capacity to insert the node.
 */
  s16(*insertFirst)(List *list, void *data,u16 size);

 /**
 * @brief Inserts a node at the end of a list.
 *
 * This function inserts the provided node at the end of the provided list.
 *
 * @param list Pointer to the list in which to insert the node.
 * @param node Pointer to the node to insert at the end of the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_NotEnoughCapacity: The list does not have enough capacity to insert the node.
 */

  s16(*insertLast)(List* list, void* data, u16 size);

 /**
 * @brief Inserts a node at a specified index in a list.
 *
 * This function inserts the provided node at the specified index in the provided list.
 *
 * @param list Pointer to the list in which to insert the node.
 * @param node Pointer to the node to insert at the specified index in the list.
 * @param index Index at which to insert the node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_InvalidIndex: The specified index is invalid.
 *         - kErrorCode_NotEnoughCapacity: The list does not have enough capacity to insert the node.
 */
  s16(*insertAt)(List *list, void* data, u16 size, u16 index);

 /**
 * @brief Extracts the first node from a list.
 *
 * This function removes and frees the memory associated with the first node in the provided list.
 *
 * @param list Pointer to the list from which to extract the first node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_ListEmpty: The list is empty.
 */
  void*(*extractFirst)(List *list);

 /**
 * @brief Extracts the last node from a list.
 *
 * This function removes and frees the memory associated with the last node in the provided list.
 *
 * @param list Pointer to the list from which to extract the last node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_ListEmpty: The list is empty.
 */
  void*(*extractLast)(List *list);
 
 /**
 * @brief Extracts a node at a specified index from a list.
 *
 * This function removes and frees the memory associated with the node at the specified index in the provided list.
 *
 * @param list Pointer to the list from which to extract the node.
 * @param index Index of the node to extract.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 *         - kErrorCode_InvalidIndex: The specified index is invalid.
 *         - kErrorCode_ListEmpty: The list is empty.
 */
  void*(*extractAt)(List *list, u16 index);

  /**
 * @brief Concatenates two lists.
 *
 * This function concatenates the provided next_list to the end of the provided list.
 *
 * @param list Pointer to the list to which the next_list will be concatenated.
 * @param next_list Pointer to the list that will be concatenated to the end of the provided list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: Either the provided list pointer or the next_list pointer is NULL.
 */
  s16(*concat)(List *list, List *next_list);

 /**
 * @brief Traverses a list and applies a callback function to each node.
 *
 * This function traverses the provided list and applies the provided callback function to each node.
 *
 * @param list Pointer to the list to traverse.
 * @param callback Pointer to the callback function to apply to each node.
 *                 The callback function should accept a pointer to a MemoryNode as its parameter.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided list pointer is NULL.
 */

  s16 (*traverse)(List *list, void (*callback)(MemoryNode *));

 /**
 * @brief Prints information about a list.
 *
 * This function prints information about the provided list.
 *
 * @param list Pointer to the list to print information about.
 */
  void(*print) (List *list);
};

List* LIST_create(u16 capacity); // Creates a new list



#endif // __ADT_LIST_H__