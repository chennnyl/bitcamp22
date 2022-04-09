/**
 * @file vec2.c
 * @author gavint
 * @brief implements vec2.h, vector functions
 * @date 2022-04-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "vec2.h"

#include <nds/arm9/math.h>

/**
 * @brief Add two vectors
 * 
 * @param v1 First vector to add
 * @param v2 Second vector to add
 * @return Vector2 
 */
Vector2 vec2_add(Vector2 v1, Vector2 v2) {
    return (Vector2) {v1.x + v2.x, v1.y + v2.y};
};

/**
 * @brief Subtract one vector from another
 * 
 * @param v1 Vector to subtract from
 * @param v2 Vector to subtract
 * @return Vector2 
 */
Vector2 vec2_sub(Vector2 v1, Vector2 v2) {
    return (Vector2) {v1.x - v2.x, v1.y - v2.y};
};

/**
 * @brief Scale a vector by a scalar
 * 
 * @param vec Vector to scale
 * @param scalar Scalar to scale by
 * @return Vector2 
 */
Vector2 vec2_scale(Vector2 vec, fixed32 scalar) {
    return (Vector2) {mulf32(vec.x, scalar), mulf32(vec.y, scalar)};
};

/**
 * @brief Normalize a vector
 * 
 * @param vec
 * @return Vector2 
 */
Vector2 vec2_norm(Vector2 vec) {
    fixed32 mag = vec2_mag(vec);
    return (Vector2) {divf32(vec.x, mag), divf32(vec.y, mag)};
};

/**
 * @brief Get the squared magnitude of a vector (faster)
 * 
 * @param vec 
 * @return fixed32 
 */
fixed32 vec2_sqrmag(Vector2 vec) {
    return mulf32(vec.x, vec.x) + mulf32(vec.y, vec.y);
};

/**
 * @brief Get the magnitude of a vector
 * 
 * @param vec 
 * @return fixed32 
 */
fixed32 vec2_mag(Vector2 vec) {
    return sqrtf32(mulf32(vec.x, vec.x) + mulf32(vec.y, vec.y));
}
