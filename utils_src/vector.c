#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


// Allocates a new vector.
// Parameters:
//   cell_size    - size of each element in bytes
//   capacity_opt - initial capacity (0 = default)
//   do_clear     - whether to zero-initialize memory
// Returns: pointer to the new vector
void *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear);

// Frees a vector.
// Parameters:
//   data    - pointer to vector
//   free_fn - optional function to free each element
void vecFree(void *data, void (*free_fn)(void *));

// Returns the number of elements currently in the vector.
uint32_t vecLength(void *data);

// Returns the size in bytes of each element.
uint16_t vecCellSize(void *data);

// Returns the current allocated capacity of the vector.
uint32_t vecCapacity(void *data);

// Sets an element at a specific index.
// Parameters:
//   data - vector pointer
//   idx  - index of the element
//   val  - pointer to the new value
void vecSet(void *data, uint32_t idx, void *val);

// Returns a pointer to the element at the specified index.
// Parameters:
//   data - vector pointer
//   idx  - index of the element
// Returns: pointer to element
void *vecAt(void *data, uint32_t idx);

// Pushes a new element to the end of the vector, growing if needed.
// Usage: vecPush(vec, &value);
#define vecPush(d, val) ((d) = __vecPush((d), (val)))

// Pops the last element from the vector.
// Returns: pointer to the popped element
void *vecPop(void *data);

// Reserves capacity for the vector.
// Parameters:
//   d   - vector pointer
//   cap - new desired capacity
//   clr - whether to zero-initialize new elements
#define vecReserve(d, cap, clr) ((d) = __vecReserve((d), (cap), (clr)))


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


typedef struct vec_meta_s {
    uint32_t  capacity;
    uint32_t    length;
    uint16_t cell_size;
} VecMeta;

#define __VEC_GET_META(d) ((VecMeta *)((char *)(d) - sizeof(VecMeta)))
#define __VEC_GET_DATA(v) ((void *)((char *)(v) + sizeof(VecMeta)))

void __vecPurge(VecMeta *v);
void *__vecPush(void *data, void *val);
void *__vecReserve(void *data, uint32_t new_capacity, bool do_clear);

void *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear) {
    assert(cell_size > 0);
    if (capacity_opt == 0) capacity_opt = 4;

    VecMeta *v = NULL;
    if (!do_clear) v = malloc(sizeof(VecMeta) + (cell_size * capacity_opt));
    else      v = calloc(1, (sizeof(VecMeta) + (cell_size * capacity_opt)));
    if (!v) return NULL;

    v->capacity = capacity_opt;
    v->length = 0;
    v->cell_size = cell_size;

    return __VEC_GET_DATA(v);
}

void vecFree(void *data, void (*free_fn)(void *)) {
    if (!data) return;

    VecMeta *v = __VEC_GET_META(data);
    if (free_fn) {
        for (size_t i = 0; i < v->length; i++) {
            void *elem = (char *)__VEC_GET_DATA(v) + i * v->cell_size;
            free_fn(*(void **)elem);
        }
    }

    if (v) free(v);
}

void __vecPurge(VecMeta *v) {
    assert(v);
    memset(v, 0, (sizeof(VecMeta) + (v->capacity * v->cell_size)));
}

uint32_t vecLength(void *data) {
    assert(data);
    VecMeta *v = __VEC_GET_META(data);
    return v->length;
}

uint32_t vecCapacity(void *data) {
    assert(data);
    VecMeta *v = __VEC_GET_META(data);
    return v->capacity;
}

uint16_t vecCellSize(void *data) {
    assert(data);
    VecMeta *v = __VEC_GET_META(data);
    return v->cell_size;
}

void vecSet(void *data, uint32_t idx, void *val) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    assert(idx < v->length);

    char *p = (char *)((char *)data + (idx * v->cell_size));
    memcpy(p, val, v->cell_size);
}

void *vecAt(void *data, uint32_t idx) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    assert(idx < v->length);

    return (void *)((char *)data + (idx * v->cell_size));
}

void *__vecPush(void *data, void *val) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    
    // Handle resizing
    if (v->length == (v->capacity)) {
        v->capacity *= 2;
        VecMeta *temp = realloc(v, sizeof(VecMeta) + (v->capacity * v->cell_size));
        if (!temp) return v;
        v = temp;
        data = __VEC_GET_DATA(v);
    }

    memcpy((char *)data + (v->length * v->cell_size), val, v->cell_size);
    v->length += 1;

    return data;
}

void *vecPop(void *data) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    v->length -= 1;
    return (void *)((char *)data + (v->length * v->cell_size));
}

void *__vecReserve(void *data, uint32_t new_capacity, bool do_clear) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    VecMeta *temp = realloc(v, sizeof(VecMeta) + (new_capacity * v->cell_size));
    if (!temp) return v;
    v = temp;

    if (do_clear) {
        memset(
            (char *)__VEC_GET_DATA(v) + v->capacity * v->cell_size,
            0, new_capacity * v->cell_size
        );
    }
    v->capacity = new_capacity;
    return __VEC_GET_DATA(v);
}