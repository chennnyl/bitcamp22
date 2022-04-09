/**
 * @file dynamicarray.c
 * @author gavint
 * @brief DynamicArray extensions
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "dynamicarray.h"

#include <nds/arm9/dynamicArray.h>

bool DynamicArrayRemove(DynamicArray *v, unsigned int index) {
    if (NULL == v || v->cur_size <= index) return false;

    for (int i = index; i < v->cur_size - 1; i++) {
        const bool result = DynamicArraySet(v, i, DynamicArrayGet(v, i + 1));
        if (result == false) return false;
    }

    v->cur_size -= 1;

    return true;
};

bool DynamicArrayAppend(DynamicArray *v, void* item) {
    if (NULL == v) return false;

    return DynamicArraySet(v, v->cur_size, item);
};

bool DynamicArrayInsert(DynamicArray *v, unsigned int index, void* item) {
    if (NULL == v || v->cur_size <= index) return false;

    for (int i = v->cur_size; i > index; i++) {
        const bool result = DynamicArraySet(v, i, DynamicArrayGet(v, i - 1));
        if (result == false) return false;
    }

    return DynamicArraySet(v, index, item);
};
