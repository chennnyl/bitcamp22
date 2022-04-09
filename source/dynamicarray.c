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

bool DArrayRemove(DArray *v, unsigned int index) {
    if (NULL == v || v->size <= index) return false;

    for (int i = index; i < v->size - 1; i++) {
        const bool result = DynamicArraySet(v, i, DynamicArrayGet(v, i + 1));
        if (result == false) return false;
    }

    v->size -= 1;

    return true;
};

bool DArrayAppend(DArray *v, void* item) {
    if (NULL == v) return false;

    const bool result = DynamicArraySet(v, v->size, item);
    v->size += 1;

    return result;
};

bool DArrayInsert(DArray *v, unsigned int index, void* item) {
    if (NULL == v || v->size <= index) return false;

    for (int i = v->size; i > index; i++) {
        const bool result = DynamicArraySet(v, i, DynamicArrayGet(v, i - 1));
        if (result == false) return false;
    }

    return DynamicArraySet(v, index, item);
};

bool DArraySet(DArray *v, unsigned int index, void* item) {
    if (NULL == v || v->size <= index) return false;

    return DynamicArraySet(v->arr, index, item);
};

void* DArrayGet(DArray *v, unsigned int index) {
    if (NULL == v || v->size <= index) return NULL;

    return DynamicArrayGet(v->arr, index);
}

DArray* DArrayInit() {
    DArray *darr = malloc(sizeof(DArray));
    if (NULL == darr) return darr;

    *darr = (DArray) {
        .arr = malloc(sizeof(DynamicArray)),
        .size = 1
    };

    DynamicArrayInit(darr->arr, 1);
    return darr;
};


