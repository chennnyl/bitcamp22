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
    DArray* colliders;
    DArray* rigidbodies;

    fixed32 gravity;
    fixed32 step;
};

struct Rigidbody {
    Collider* col;
    Physics* engine;
    fixed32 mass;
    fixed32 inv_mass;
    Vector2 vel;
};

struct Collider {
    Physics* engine;
    Vector2 pos;
    Vector2 size;
};

Physics* phys_Construct(fixed32 gravity) {
    Physics* engine = malloc(sizeof(Physics));

    if (NULL == engine) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    *engine = (Physics) {0};

    engine->gravity = gravity;
    engine->colliders = DArrayInit();
    engine->rigidbodies = DArrayInit();

    /* if collider or rididbody array returned null */
    if (NULL == engine->colliders || NULL == engine->rigidbodies) {
        fprintf(stderr, "[Log] Dynamic array failed to initialize");
        WHERE
        exit(GENERIC_ERROR);
    }

    return engine;
};

void phys_Destroy(Physics* engine) {
    if (NULL == engine) return;

    {
        int i = engine->colliders->size;
        while (i --> 0) {
            phys_col_Destroy(DArrayGet(engine->colliders, i));
        };
    }

    {
        int i = engine->rigidbodies->size;
        while (i --> 0) {
            phys_rb_Destroy(DArrayGet(engine->rigidbodies, i));
        };
    }

    free(engine);
};

void phys_setGravity(Physics* engine, fixed32 gravity) {
    engine->gravity = gravity;
};

fixed32 phys_getGravity(Physics* engine) {
    return engine->gravity;
};

static void apply_gravity(Physics* engine) {
    for (int i = 0; i < engine->rigidbodies->size; i++) {
        Rigidbody* rb = DArrayGet(engine->rigidbodies, i);
        phys_rb_addForce(rb, (Vector2) {0, mulf32(engine->gravity, rb->mass)});
    }
};

void phys_step(Physics* engine, fixed32 step) {
    engine->step = step;

    apply_gravity(engine);

    for (int i = 0; i < engine->rigidbodies->size; i++) {
        Rigidbody *rb = DArrayGet(engine->rigidbodies, i);
        rb->col->pos = vec2_add(rb->col->pos, vec2_scale(rb->vel, step));
    };

    for (int irb = 0; irb < engine->rigidbodies->size; irb++) {
        const Rigidbody* rb = DArrayGet(engine->rigidbodies, irb);

        for (int icol = 0; icol < engine->colliders->size; icol++) {
            Collider* col = DArrayGet(engine->colliders, icol);

            if (phys_col_colliding(rb->col, col)) {
                fprintf(stderr, "[Log] something is colliding idk");
                WHERE;
            };
        }
    };
};

Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass) {
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    if (NULL == rb) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    *rb = (Rigidbody) {0};

    phys_rb_setCol(rb, col);
    rb->engine = engine;
    DArrayAppend(engine->rigidbodies, rb);
    phys_rb_setMass(rb, mass);

    return rb;
}

void phys_rb_Destroy(Rigidbody* rb) {
    if (rb == NULL) return;

    /* Remove rb from array in engine */
    DArray* arr = rb->engine->rigidbodies;
    
    int index = arr->size;
    while (index --> 0) {
        if (DArrayGet(arr, index) == rb) {
            DArrayRemove(arr, index);
            break;
        }
    }

    /* Free rb */
    free(rb);
};

Collider* phys_rb_getCol(Rigidbody* rb) {return rb->col;};
fixed32 phys_rb_getMass(Rigidbody* rb) {return rb->mass;};
void phys_rb_setMass(Rigidbody* rb, fixed32 mass) {
    rb->mass = mass;
    rb->inv_mass = divf32(inttof32(1), mass);
};
void phys_rb_setCol(Rigidbody* rb, Collider* col) {rb->col = col;};

void phys_rb_addForce(Rigidbody* rb, Vector2 force) {
    rb->vel = vec2_add(rb->vel, vec2_scale(force, mulf32(rb->inv_mass, rb->engine->step)));
}

Collider* phys_col_Construct(Physics* engine, Vector2 pos, Vector2 size) {
    Collider* col = malloc(sizeof(Collider));

    if (NULL == col) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    *col = (Collider) {0};

    col->engine = engine;
    DArrayAppend(engine->colliders, col);
    phys_col_setPos(col, pos);
    phys_col_setSize(col, size);

    return col;
};

void phys_col_Destroy(Collider* col) {
    if (col == NULL) return;

    /* Remove collider from array in engine */
    DArray* arr = col->engine->colliders;
    
    int index = arr->size;
    while (index --> 0) {
        if (DArrayGet(arr, index) == col) {
            DArrayRemove(arr, index);
            break;
        }
    }

    /* Free collider */
    free(col);
};

Vector2 phys_col_getPos(Collider* col) {return col->pos;};
Vector2 phys_col_getSize(Collider* col) {return col->size;};
void phys_col_setPos(Collider* col, Vector2 pos) {col->pos = pos;};
void phys_col_setSize(Collider* col, Vector2 size) {col->size = size;};

bool phys_col_colliding(Collider* col1, Collider* col2) {
    Vector2 pos1 = col1->pos;
    Vector2 pos2 = col2->pos;

    return pos1.x <= phys_col_x2(col2) &&
           phys_col_x2(col1) >= pos2.x &&
           pos1.y >= phys_col_y2(col2) &&
           phys_col_y2(col1) <= pos2.y;
};
