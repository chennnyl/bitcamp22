/**
 * @file physics.c
 * @author your name (you@domain.com)
 * @brief physics yay fun
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "physics.h"

#include <nds/arm9/math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dynamicarray.h"
#include "exitcodes.h"
#include "util.h"

struct Physics {
    DynamicArray* colliders;
    DynamicArray* rigidbodies;
};

struct Rigidbody {
    Collider* col;
    Physics* engine;
    fixed32 mass;
    fixed32 inv_mass;
};

struct Collider {
    Physics* engine;
    Vector2 pos;
    Vector2 size;
};

Physics* phys_Construct() {
    Physics* engine = malloc(sizeof(Physics));

    if (NULL == engine) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    engine->colliders = DynamicArrayInit(malloc(sizeof(DynamicArray)), 1);
    engine->rigidbodies = DynamicArrayInit(malloc(sizeof(DynamicArray)), 1);

    /* if collider or rididbody array returned null */
    if (NULL == engine->colliders || NULL == engine->rigidbodies) {
        fprintf(stderr, "[Log] Dynamic array failed to initialize");
        WHERE
        exit(GENERIC_ERROR);
    }
};

void phys_Destroy(Physics* engine) {
    if (NULL == engine) return;

    {
        int i = engine->colliders->cur_size;
        while (i --> 0) {
            phys_col_Destroy(DynamicArrayGet(engine->colliders, i));
        };
    }

    {
        int i = engine->rigidbodies->cur_size;
        while (i --> 0) {
            phys_rb_Destroy(DynamicArrayGet(engine->rigidbodies, i));
        };
    }

    free(engine);
};

Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass) {
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    if (NULL == rb) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    phys_rb_setcol(rb, col);
    rb->engine = engine;
    phys_rb_setmass(rb, mass);

    return rb;
}

void phys_rb_Destroy(Rigidbody* rb) {
    if (rb == NULL) return;

    /* Remove rb from array in engine */
    const DynamicArray* arr = rb->engine->rigidbodies;
    
    int index = arr->cur_size;
    while (index --> 0) {
        if (DynamicArrayGet(arr, index) == rb) {
            DynamicArrayRemove(arr, index);
            break;
        }
    }

    /* Free rb */
    free(rb);
};

Collider* phys_rb_getcol(Rigidbody* rb) {return rb->col;};
fixed32 phys_rb_getmass(Rigidbody* rb) {return rb->mass;};
void phys_rb_setmass(Rigidbody* rb, fixed32 mass) {
    rb->mass = mass;
    rb->inv_mass = divf32(1, mass);
};
void phys_rb_setcol(Rigidbody* rb, Collider* col) {rb->col = col;};

Collider* phys_col_Construct(Physics* engine, Vector2 pos, Vector2 size) {
    Collider* col = malloc(sizeof(Collider));

    if (NULL == col) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    col->engine = engine;
    phys_col_setpos(col, pos);
    phys_col_setsize(col, size);
};

void phys_col_Destroy(Collider* col) {
    if (col == NULL) return;

    /* Remove collider from array in engine */
    const DynamicArray* arr = col->engine->colliders;
    
    int index = arr->cur_size;
    while (index --> 0) {
        if (DynamicArrayGet(arr, index) == col) {
            DynamicArrayRemove(arr, index);
            break;
        }
    }

    /* Free collider */
    free(col);
};

Vector2 phys_col_getpos(Collider* col) {return col->pos;};
Vector2 phys_col_getsize(Collider* col) {return col->size;};
void phys_col_setpos(Collider* col, Vector2 pos) {col->pos = pos;};
void phys_col_setsize(Collider* col, Vector2 size) {col->size = size;};
