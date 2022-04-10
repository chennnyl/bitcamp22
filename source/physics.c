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
    fixed32 resistance;
};

struct Rigidbody {
    Collider* col;
    Physics* engine;
    fixed32 mass;
    fixed32 inv_mass;
    fixed32 elasticity;
    Vector2 vel;
};

struct Collider {
    Physics* engine;
    Vector2 pos;
    Vector2 size;
};

Physics* phys_Construct(fixed32 gravity, fixed32 resistance) {
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
    engine->resistance = resistance;

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
        phys_rb_addForce(rb, (Vector2) {0, mulf32(-engine->gravity, rb->mass)});
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
        Rigidbody* rb = DArrayGet(engine->rigidbodies, irb);

        for (int icol = 0; icol < engine->colliders->size; icol++) {
            Collider* col = DArrayGet(engine->colliders, icol);

            if (col == rb->col) continue;

            if (phys_col_colliding(rb->col, col)) {
                //const Vector2 direction = vec2_scale(vec2_norm(rb->vel), inttof32(-1));
                Vector2 direction = {0};
                if (col->pos.x >= rb->col->pos.x &&
                    col->pos.x < phys_col_x2(rb->col)) {
                    direction.x -= inttof32(1);
                }
                if (phys_col_x2(col) > rb->col->pos.x &&
                    phys_col_x2(col) <= phys_col_x2(rb->col)) {
                    direction.x += inttof32(1);
                }
                if (col->pos.y >= rb->col->pos.y &&
                    col->pos.y < phys_col_y2(rb->col)) {
                    direction.y -= inttof32(1);
                }
                if (phys_col_y2(col) > rb->col->pos.y &&
                    phys_col_y2(col) <= phys_col_y2(rb->col)) {
                    direction.y += inttof32(1);
                }

                fprintf(stderr, "[LOG] direction.x = %ld, direction.y = %ld\n", f32toint(direction.x), f32toint(direction.y));

                fixed32 distance_x = 0;
                fixed32 distance_y = 0;

                if (direction.x > 0) {
                    distance_x = col->pos.x - phys_col_x2(rb->col);
                } else if (direction.x < 0) {
                    distance_x = rb->col->pos.x - phys_col_x2(col);
                }

                if (direction.y > 0) {
                    distance_y = phys_col_y2(col) - rb->col->pos.y;
                } else if (direction.y < 0) {
                    distance_y = phys_col_y2(rb->col) - col->pos.y;
                }

                fixed32 vel_x = 0;
                fixed32 vel_y = 0;
                if (direction.x != 0) vel_x = mulf32(abs(distance_x) + 0xF, direction.x) - rb->vel.x;
                if (direction.y != 0) vel_y = mulf32(abs(distance_y) + 0xF, direction.y) - rb->vel.y;
                const Vector2 velocity = {vel_x, vel_y};
                const Vector2 acceleration = vec2_scale(velocity, divf32(inttof32(1), engine->step));
                const Vector2 accel_grav = vec2_add(acceleration, (Vector2) {0, engine->gravity});
                const Vector2 force = vec2_scale(accel_grav, rb->mass);

                phys_rb_addForce(rb, force);
            }
        }
    };
};

Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass, fixed32 elasticity) {
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    if (NULL == rb) { /* if malloc returned null */
        fprintf(stderr, "[Log] Malloc returned null");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    *rb = (Rigidbody) {0};

    phys_rb_setCol(rb, col);
    rb->engine = engine;
    rb->elasticity = elasticity;
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
    rb->vel = vec2_add(rb->vel, vec2_scale(vec2_scale(force, rb->inv_mass), rb->engine->step));
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
           pos1.y <= phys_col_y2(col2) &&
           phys_col_y2(col1) >= pos2.y;
};
