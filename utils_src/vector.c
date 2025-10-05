/* =====================================================================
 *
 * This module is a part of:
 * "deltautils.h" - General-purpose utility library for C
 *
 * Source Code: https://github.com/Delta7Actual/Delta-Utils
 * Created and maintained by Dror Sheffer
 *
 * Licensed under the MIT License.
 * See the accompanying LICENSE file for full terms.
 *
 * =====================================================================
 */


#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


// The vector struct
typedef struct vec_meta_s {
    uint32_t  capacity;  // How many cells can the vector hold before resizing
    uint32_t    length;  // The length of the vector
    uint16_t cell_size;  // The size of each cell in bytes
    void *        data;  // A pointer to the data stored in the vector
} Vector;


// Allocates a new vector.
// Parameters:
//   cell_size    - size of each element in bytes (must be > 0)
//   capacity_opt - initial capacity (0 = default of 4)
//   do_clear     - whether to zero-initialize allocated memory
// Returns: pointer to the new vector, or NULL on allocation failure.
Vector *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear);

// Frees a vector and its internal buffer.
// Parameters:
//   vec        - pointer to vector
//   purge_data - if true, zeroes out vector data before freeing
void vecFree(Vector *vec, bool purge_data);

// Returns the number of elements currently stored in the vector.
uint32_t vecLength(const Vector *vec);

// Returns the size in bytes of each element.
uint16_t vecCellSize(const Vector *vec);

// Returns the current allocated capacity of the vector.
uint32_t vecCapacity(const Vector *vec);

// Sets an element at a specific index.
// Parameters:
//   vec - vector pointer
//   idx - index of the element (must be < vecLength())
//   val - pointer to the new value
void vecSet(Vector *vec, uint32_t idx, const void *val);

// Returns a pointer to the element at the specified index.
// Parameters:
//   vec - vector pointer
//   idx - index of the element (must be < vecLength())
// Returns: pointer to element
void *vecAt(Vector *vec, uint32_t idx);

// Pushes a new element to the end of the vector, growing capacity if needed.
// Parameters:
//   vec - vector pointer
//   val - pointer to the value to append
// Returns: true on success, false if reallocation failed.
void vecPush(Vector *vec, const void *val);

// Pops the last element from the vector.
// Parameters:
//   vec - vector pointer (must have at least 1 element)
// Returns: pointer to the popped element inside the buffer.
// NOTE: The returned pointer becomes invalid if the vector is resized.
void *vecPop(Vector *vec);

// Ensures the vector has at least 'new_capacity' slots allocated.
// Parameters:
//   vec          - vector pointer
//   new_capacity - desired minimum capacity
//   do_clear     - if true, zero-initializes newly allocated space
// Returns: true on success, false if reallocation failed.
void vecReserve(Vector *vec, uint32_t new_capacity, bool do_clear);


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


void __vecPurge(Vector *vec) {
    assert(vec);
    memset(vec->data, 0, vec->cell_size * vec->capacity);
}

Vector *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear) {
    assert(cell_size > 0);
    if (capacity_opt == 0) capacity_opt = 4;

    Vector *v = (Vector *)malloc(sizeof(Vector));
    if (!v) return NULL;

    v->capacity = capacity_opt;
    v->length = 0;
    v->cell_size = cell_size;
    
    v->data = malloc(v->cell_size * v->capacity);
    if (!v->data) {
        free(v);
        return NULL;
    }

    if (do_clear) __vecPurge(v);
    return v;
}

void vecFree(Vector *vec, bool purge_data) {
    if (!vec) return;
    if (purge_data) __vecPurge(vec);
    free(vec->data);
    free(vec);
}

void vecSet(Vector *vec, uint32_t idx, const void *val) {
    assert(vec && idx < vec->length);
    memcpy((char *)vec->data + (vec->cell_size * idx), val, vec->cell_size);
}

void *vecAt(Vector *vec, uint32_t idx) {
    assert(vec && idx < vec->length);
    return (void *)((char *)vec->data + (vec->cell_size * idx));
}

void vecPush(Vector *vec, const void *val) {
    assert(vec);

    // Handle resizing
    if (vec->length == vec->capacity) {
        vec->capacity *= 2;
        void *temp = realloc(vec->data, vec->cell_size * vec->capacity);
        if (!temp) return;
        vec->data = temp;
    }

    vec->length++;
    vecSet(vec, vec->length-1, val);
}

void *vecPop(Vector *vec) {
    assert(vec && vec->length > 0);

    void *val = vecAt(vec, vec->length-1);
    vec->length--;

    return val;
}

void vecReserve(Vector *vec, uint32_t new_capacity, bool do_clear) {
    assert(vec);

    void *temp = realloc(vec->data, new_capacity * vec->cell_size);
    if (!temp) return;
    
    if (do_clear) memset(
        (char *)vec->data + vec->capacity * vec->cell_size,
        0, (new_capacity - vec->capacity) * vec->cell_size
    );
    
    vec->data = temp;
    vec->capacity = new_capacity;
}


/* =====================================================================
 *
 * MIT License
 *
 * Copyright (c) 2025 Dror Sheffer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * =====================================================================
 */