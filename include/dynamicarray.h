/**
 * @file dynamicarray.h
 * @author gavint
 * @brief some helpful extensions to DynamicArray
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <nds/arm9/dynamicArray.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Remove index from the array and shift all other values down
 * 
 * @param v 
 * @param index 
 * @return false if v is null or if index is larger than the array
 */
bool DynamicArrayRemove(DynamicArray *v, unsigned int index);

/**
 * @brief Append item to end of array
 * 
 * @param v     The array to append to
 * @param item  The item to be appended
 * @return false if v is NULL or there isn't enough memory
 */
bool DynamicArrayAppend(DynamicArray *v, void* item);

/**
 * @brief Inserts item at index, shifting all other items forward
 * 
 * @param v     The array to insert
 * @param index The index to insert at
 * @param item  The item to insert at index
 * @return false if v is NULL or there isn't enough memory
 */
bool DynamicArrayInsert(DynamicArray *v, unsigned int index, void* item);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DYNAMICARRAY_H */
