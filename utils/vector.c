#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct vec_meta_s {
    uint32_t capacity;
    uint32_t length;
    uint16_t cell_size;
} VecMeta;

#define __VEC_GET_META(d) ((VecMeta *)((char *)(d) - sizeof(VecMeta)))
#define __VEC_GET_DATA(v) ((void *)((char *)(v) + sizeof(VecMeta)))

void *vecInit(uint16_t cell_size, uint32_t capacity_opt, bool do_clear);
void vecFree(void *data, bool do_purge);
void __vecPurge(VecMeta *v);
void vecSet(void *data, uint32_t idx, void *val);
void *vecAt(void *data, uint32_t idx);

#define vecPush(d, val) ((d) = __vecPush((d), (val)))
void *__vecPush(void *data, void *val);

void *vecPop(void *data);

#define vecReserve(d, cap, clr) ((d) = __vecReserve((d), (cap), (clr)))
void *__vecReserve(void *data, uint32_t new_capacity, bool do_clear);z

void *vecInit(uint16_t cell_size, uint32_t capacity_opt, bool do_clear) {
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

void vecFree(void *data, bool do_purge) {
    if (!data) return;

    VecMeta *v = __VEC_GET_META(data);
    if (do_purge) __vecPurge(v);

    if (v) free(v);
}

void __vecPurge(VecMeta *v) {
    assert(v);
    memset(v, 0, (sizeof(VecMeta) + (v->capacity * v->cell_size)));
}

void vecSet(void *data, uint32_t idx, void *val) {
    assert(data);

    VecMeta *v = __VEC_GET_META(data);
    assert(idx < v->length);

    char *p = (char *)(data + (idx * v->cell_size));
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
            __VEC_GET_DATA(v) + v->capacity * v->cell_size,
            0, new_capacity * v->cell_size
        );
    }
    v->capacity = new_capacity;
    return __VEC_GET_DATA(v);
}

int main(void) {
    // Create a vector of ints
    int *vec = vecInit(sizeof(int), 4, true);
    assert(vec);

    // Push values
    for (int i = 0; i < 10; i++) {
        int val = i * 10;
        vecPush(vec, &val);
    }

    // Print values
    printf("Vector contents:\n");
    for (uint32_t i = 0; i < 10; i++) {
        int val = *(int *)vecAt(vec, i);
        printf("vec[%u] = %d\n", i, val);
    }

    // Modify some values
    int new_val = 999;
    vecSet(vec, 5, &new_val);

    printf("\nAfter vecSet(vec, 5, 999):\n");
    for (uint32_t i = 0; i < 10; i++) {
        int val = *(int *)vecAt(vec, i);
        printf("vec[%u] = %d\n", i, val);
    }

    // Reserve more capacity
    vec = vecReserve(vec, 20, true);
    printf("\nAfter vecReserve(vec, 20, true), capacity increased.\n");

    // Pop values
    printf("\nPopping last 3 values:\n");
    for (int i = 0; i < 3; i++) {
        int val = *(int *)vecPop(vec);
        printf("Popped: %d\n", val);
    }

    // Final vector contents
    printf("\nFinal vector contents:\n");
    VecMeta *vmeta = __VEC_GET_META(vec);
    for (uint32_t i = 0; i < vmeta->length; i++) {
        int val = *(int *)vecAt(vec, i);
        printf("vec[%u] = %d\n", i, val);
    }

    // Free vector
    vecFree(vec, true);

    return 0;
}