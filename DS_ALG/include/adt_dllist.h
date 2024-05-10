/**
 * @file adt_dllist.h
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
  struct dllist_ops_s *ops_;
} DLList;

// Memory Node's API Declarations

struct dllist_ops_s {
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
 * @brief Destroy a DLListand free the allocated memory.
 *
 * This function destroys the specified DLListand frees the memory associated with it.
 *
 * @param DLListPointer to the DLListto be destroyed.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the DLListwas successfully destroyed.
 *         - `kErrorCode_ListNull` if the `list` pointer is NULL.
 */
  s16(*destroy)(DLList*list);

/**
 * @brief Soft reset for a list, clearing all nodes and resetting DLListproperties.
 *
 * This function performs a soft reset for the specified list, clearing all nodes and resetting the DLListproperties.
 *
 * @param DLListPointer to the DLListto be reset.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the soft reset was successful.
 *         - `kErrorCode_ListNull` if the `list` pointer is NULL.
 */

  s16(*softReset)(DLList*List);


/**
 * @brief Reset a list, clearing all nodes and resetting DLListproperties.
 *
 * This function resets the specified list, clearing all nodes and resetting the DLListproperties.
 *
 * @param DLListPointer to the DLListto be reset.
 * @return Returns an error code indicating the result of the operation.
 *         - `kErrorCode_Ok` if the reset was successful.
 *         - `kErrorCode_ListNull` if the `list` pointer is NULL.
 */
  s16(*reset)(DLList*list);

  /**
 * @brief Resizes the capacity of a list.
 *
 * This function resizes the capacity of the provided DLListto the specified new capacity.
 * If the new capacity is greater than the current capacity, the function updates the capacity of the list.
 * If the new capacity is less than the current capacity, the function removes nodes from the end of the list
 * until the capacity matches the new capacity, freeing memory associated with removed nodes.
 *
 * @param DLListPointer to the DLListto resize.
 * @param new_capacity The new capacity to set for the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 */
  s16(*resize)(DLList*list, u16 new_capacity);

 /**
 * @brief Retrieves the capacity of a list.
 *
 * This function returns the current capacity of the provided list.
 *
 * @param DLListPointer to the DLListfrom which to retrieve the capacity.
 * @return The capacity of the list, or 0 if the provided DLListpointer is NULL.
 */
  u16(*capacity)(DLList*list);//

 /**
 * @brief Retrieves the length of a list.
 *
 * This function returns the current length of the provided list.
 *
 * @param DLListPointer to the DLListfrom which to retrieve the length.
 * @return The length of the list, or 0 if the provided DLListpointer is NULL.
 */
  u16(*length)(DLList*list);//


 /**
 * @brief Checks if a DLListis empty.
 *
 * This function checks whether the provided DLListis empty or not.
 *
 * @param DLListPointer to the DLListto check.
 * @return True if the DLListis empty, False otherwise. If the provided DLListpointer is NULL, False is returned.
 */
  boolean(*isEmpty)(DLList*list);

 /**
 * @brief Checks if a DLListis full.
 *
 * This function checks whether the provided DLListis full or not.
 *
 * @param DLListPointer to the DLListto check.
 * @return True if the DLListis full, False otherwise. If the provided DLListpointer is NULL, False is returned.
 */
  boolean(*isFull)(DLList*list);

/**
 * @brief Retrieves the data stored in the first node of a list.
 *
 * This function returns a pointer to the data stored in the first node of the provided list.
 *
 * @param DLListPointer to the DLListfrom which to retrieve the data from the first node.
 * @return Pointer to the data stored in the first node of the list. 
 *         If the provided DLListpointer is NULL, kErrorCode_ListNull is returned.
 *         If the first node of the DLListis NULL, kErrorCode_FirstNull is returned.
 */
  void*(*first)(DLList*list);

 /**
 * @brief Retrieves the data stored in the last node of a list.
 *
 * This function returns a pointer to the data stored in the last node of the provided list.
 *
 * @param DLListPointer to the DLListfrom which to retrieve the data from the last node.
 * @return Pointer to the data stored in the last node of the list. 
 *         If the provided DLListpointer is NULL, kErrorCode_ListNull is returned.
 *         If the last node of the DLListis NULL, kErrorCode_LastNull is returned.
 */
  void*(*last)(DLList*list);

 /**
 * @brief Retrieves the data stored in the node at a specified index in a list.
 *
 * This function returns a pointer to the data stored in the node at the specified index in the provided list.
 *
 * @param DLListPointer to the DLListfrom which to retrieve the data from the node at the specified index.
 * @param index Index of the node from which to retrieve the data.
 * @return Pointer to the data stored in the node at the specified index in the list. 
 *         If the provided DLListpointer is NULL, kErrorCode_ListNull is returned.
 *         If the index is out of bounds, the pointer to the node at that index is returned.
 */
  void*(*at)(DLList*list, u16 index);

 /**
 * @brief Inserts a node at the beginning of a list.
 *
 * This function inserts the provided node at the beginning of the provided list.
 *
 * @param DLListPointer to the DLListin which to insert the node.
 * @param node Pointer to the node to insert at the beginning of the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_NotEnoughCapacity: The DLListdoes not have enough capacity to insert the node.
 */
  s16(*insertFirst)(DLList*list, void *data,u16 size);

 /**
 * @brief Inserts a node at the end of a list.
 *
 * This function inserts the provided node at the end of the provided list.
 *
 * @param DLListPointer to the DLListin which to insert the node.
 * @param node Pointer to the node to insert at the end of the list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_NotEnoughCapacity: The DLListdoes not have enough capacity to insert the node.
 */

  s16(*insertLast)(DLList* list, void* data, u16 size);

 /**
 * @brief Inserts a node at a specified index in a list.
 *
 * This function inserts the provided node at the specified index in the provided list.
 *
 * @param DLListPointer to the DLListin which to insert the node.
 * @param node Pointer to the node to insert at the specified index in the list.
 * @param index Index at which to insert the node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_NodeNull: The provided node pointer is NULL.
 *         - kErrorCode_InvalidIndex: The specified index is invalid.
 *         - kErrorCode_NotEnoughCapacity: The DLListdoes not have enough capacity to insert the node.
 */
  s16(*insertAt)(DLList*list, void* data, u16 size, u16 index);

 /**
 * @brief Extracts the first node from a list.
 *
 * This function removes and frees the memory associated with the first node in the provided list.
 *
 * @param DLListPointer to the DLListfrom which to extract the first node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_ListEmpty: The DLListis empty.
 */
  void*(*extractFirst)(DLList*list);

 /**
 * @brief Extracts the last node from a list.
 *
 * This function removes and frees the memory associated with the last node in the provided list.
 *
 * @param DLListPointer to the DLListfrom which to extract the last node.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_ListEmpty: The DLListis empty.
 */
  void*(*extractLast)(DLList*list);
 
 /**
 * @brief Extracts a node at a specified index from a list.
 *
 * This function removes and frees the memory associated with the node at the specified index in the provided list.
 *
 * @param DLListPointer to the DLListfrom which to extract the node.
 * @param index Index of the node to extract.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 *         - kErrorCode_InvalidIndex: The specified index is invalid.
 *         - kErrorCode_ListEmpty: The DLListis empty.
 */
  void*(*extractAt)(DLList*list, u16 index);

  /**
 * @brief Concatenates two lists.
 *
 * This function concatenates the provided next_DLListto the end of the provided list.
 *
 * @param DLListPointer to the DLListto which the next_DLListwill be concatenated.
 * @param next_DLListPointer to the DLListthat will be concatenated to the end of the provided list.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: Either the provided DLListpointer or the next_DLListpointer is NULL.
 */
  s16(*concat)(DLList*list, DLList*next_list);

 /**
 * @brief Traverses a DLListand applies a callback function to each node.
 *
 * This function traverses the provided DLListand applies the provided callback function to each node.
 *
 * @param DLListPointer to the DLListto traverse.
 * @param callback Pointer to the callback function to apply to each node.
 *                 The callback function should accept a pointer to a MemoryNode as its parameter.
 * @return Error code indicating the success or failure of the operation.
 *         - kErrorCode_Ok: Operation completed successfully.
 *         - kErrorCode_ListNull: The provided DLListpointer is NULL.
 */

  s16 (*traverse)(DLList*list, void (*callback)(MemoryNode *));

 /**
 * @brief Prints information about a list.
 *
 * This function prints information about the provided list.
 *
 * @param DLListPointer to the DLListto print information about.
 */
  void(*print) (DLList*list);
};

DLList* DLList_create(u16 capacity); // Creates a new list



#endif // __ADT_LIST_H__