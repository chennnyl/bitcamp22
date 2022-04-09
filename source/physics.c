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

#include <nds/arm9/dynamicArray.h>
#include <nds/arm9/math.h>
#include <stdio.h>
#include <stdlib.h>

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

    DynamicArrayInit(engine->colliders, 1);
    DynamicArrayInit(engine->rigidbodies, 1);

    /* if collider or rididbody array returned null */
    if (NULL == engine->colliders || NULL == engine->rigidbodies) {
        fprintf(stderr, "[Log] Dynamic array failed to initialize");
        WHERE
        exit(GENERIC_ERROR);
    }
};

void phys_Destroy(Physics* engine) {
    
};

Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass) {
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    if (NULL == rb) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    phys_rb_setcol(col);
    rb->engine = engine;
    phys_rb_setmass(rb, mass);

    return rb;
}

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

/*void phys_col_Destroy(Collider* col) {
    col->engine->colliders
};*/

Vector2 phys_col_getpos(Collider* col) {return col->pos;};
Vector2 phys_col_getsize(Collider* col) {return col->size;};
void phys_col_setpos(Collider* col, Vector2 pos) {col->pos = pos;};
void phys_col_setsize(Collider* col, Vector2 size) {col->size = size;};
