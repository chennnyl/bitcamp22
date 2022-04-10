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
Physics* phys_Construct(fixed32 gravity);
/**
 * @brief Destroys the physics engine instance. Uses free()
 * 
 */
void phys_Destroy(Physics*);
void phys_setGravity(Physics*, fixed32 gravity);
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
Rigidbody* phys_rb_Construct(Physics* engine, Collider* col, fixed32 mass, fixed32 elasticity);
#define RigidbodyCreator(engine, pos, size, mass, elasticity) (phys_rb_Construct(engine, phys_col_Construct(engine, pos, size), mass, elasticity))
/**
 * @brief Destroys the rigidbody instance. Uses free()
 * 
 */
void phys_rb_Destroy(Rigidbody*);
Collider* phys_rb_getCol(Rigidbody*);
fixed32 phys_rb_getMass(Rigidbody*);
void phys_rb_setMass(Rigidbody*, fixed32 mass);
/**
 * @brief Sets the Rigidbody's collider. Note that this
 *  does not delete the old collider.
 * 
 * @param col 
 */
void phys_rb_setCol(Rigidbody*, Collider* col);
void phys_rb_addForce(Rigidbody*, Vector2 force);

/**
 * @brief Creates a new collider instance. Uses malloc()
 * 
 * @param engine 
 * @return Collider* 
 */
Collider* phys_col_Construct(Physics* engine, Vector2 pos, Vector2 size);
/**
 * @brief Destroys the collider instance. Uses free()
 * 
 */
void phys_col_Destroy(Collider*);
Vector2 phys_col_getPos(Collider*);
Vector2 phys_col_getSize(Collider*);
void phys_col_setPos(Collider*, Vector2 pos);
void phys_col_setSize(Collider*, Vector2 size);
bool phys_col_colliding(Collider*, Collider*);

/**
 * @brief Returns the far x coordinate of collider
 * 
 */
#define phys_col_x2(collider) (phys_col_getPos(collider).x + phys_col_getSize(collider).x)
/**
 * @brief Returns the far y coordinate of collider
 * 
 */
#define phys_col_y2(collider) (phys_col_getPos(collider).y + phys_col_getSize(collider).y)

/*#define phys_col_xmid(collider) (phys_col_getPos(collider).x + mulf32(phys_col_getSize(collider).x, floattof32(0.5f)))
#define phys_col_xmid(collider) (phys_col_getPos(collider).y + mulf32(phys_col_getSize(collider).y, floattof32(0.5f)))*/

#ifdef __cplusplus
}
#endif

#endif /* PHYSICS_H */
