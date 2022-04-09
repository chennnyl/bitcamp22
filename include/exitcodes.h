#ifndef EXITCODES_H
#define EXITCODES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum exitcode {
    GENERIC_ERROR = -1,
    OUT_OF_MEMORY_ERROR = -2,
} exitcode_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EXITCODES_H */
