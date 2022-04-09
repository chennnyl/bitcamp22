/**
 * @file physics.h
 * @author gavint
 * @brief physics
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Physics engine. Create with phys_Construct();
 * 
 */
typedef struct Physics Physics;

typedef struct Collider Collider;

typedef struct Rigidbody Rigidbody;

/**
 * @brief Creates a new physics engine instance. Uses malloc()
 * 
 * @return Physics* 
 */
Physics* phys_Construct();
/**
 * @brief Destroys the physics engine instance. Uses dealloc()
 * 
 */
void phys_Destroy(Physics*);
/**
 * @brief Steps the physics engine
 * 
 * @param engine 
 * @param step amount of time to step forward
 */
void phys_step(Physics* engine, fixed32 step);

/**
 * @brief Creates a rigidbody instance. Uses malloc()
 * 
 * @param engine 
 * @return Rigidbody* 
 */
Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass);
/**
 * @brief Destroys the rigidbody instance. Uses calloc()
 * 
 */
void phys_rb_Destroy(Rigidbody*);
Collider* phys_rb_getcol(Rigidbody*);
fixed32 phys_rb_getmass(Rigidbody*);
void phys_rb_setmass(Rigidbody*, fixed32 mass);
/**
 * @brief Sets the Rigidbody's collider. Note that this
 *  does not delete the old collider.
 * 
 * @param col 
 */
void phys_rb_setcol(Rigidbody*, Collider* col);

/**
 * @brief Creates a new collider instance. Uses malloc()
 * 
 * @param engine 
 * @return Collider* 
 */
Collider* phys_col_Construct(Physics* engine, Vector2 pos, Vector2 size);
/**
 * @brief Destroys the collider instance. Uses dealloc()
 * 
 */
void phys_col_Destroy(Collider*);
Vector2 phys_col_getpos(Collider*);
Vector2 phys_col_getsize(Collider*);
void phys_col_setpos(Collider*, Vector2 pos);
void phys_col_setsize(Collider*, Vector2 size);
/**
 * @brief Returns the far x coordinate of collider
 * 
 */
#define phys_col_x2(collider) (phys_col_getpos(collider).x + phys_col_getsize(collider).x)
/**
 * @brief Returns the far y coordinate of collider
 * 
 */
#define phys_col_y2(collider) (phys_col_getpos(collider).y + phys_col_getsize(collider).y)

#ifdef __cplusplus
}
#endif

#endif /* PHYSICS_H */
