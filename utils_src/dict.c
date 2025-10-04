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


#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


#define DU_NBUCKET_SMALL    32
#define DU_NBUCKET_MEDIUM  128
#define DU_NBUCKET_BIG    1024
#define DU_NBUCKET_LARGE  4096

// A single entry in a dictionary bucket
typedef struct entry_s {
    void            *key;      // Pointer to the key
    size_t           key_len;  // Length of the key in bytes
    void            *val;      // Pointer to the value
    struct entry_s  *next;     // Next entry in the bucket's linked list
} DictEntry;

// Hash table mapping arbitrary byte-sequence keys to values
typedef struct dict_s {
    DictEntry      **entries;  // Array of bucket pointers
    uint32_t         size;     // Number of buckets
    void           (*free_key)(void *); // Callback to free keys
    void           (*free_val)(void *); // Callback to free values
} Dictionary;


/**
 * dictNew:
 *   Creates a new dictionary with a fixed-size bucket array
 *
 * Parameters:
 *   size     - Number of buckets (use DU_NBUCKET_*)
 *   free_key - Callback to free keys when removed or replaced
 *   free_val - Callback to free values when removed or replaced
 *
 * Returns:
 *   Pointer to a new Dictionary on success.
 *   NULL on allocation failure.
 */
Dictionary *dictNew(uint32_t size, void (*free_key)(void *), void (*free_val)(void *));


/**
 * dictSet:
 *   Inserts or updates a key/value pair in the dictionary.
 *
 * Parameters:
 *   dict    - Target dictionary.
 *   key     - Key pointer (will be copied as-is, not duplicated).
 *   key_len - Length of the key in bytes.
 *   val     - Value pointer.
 */
void dictSet(Dictionary *dict, void *key, size_t key_len, void *val);


/**
 * dictGet:
 *   Retrieves a value by key.
 *
 * Parameters:
 *   dict    - Target dictionary.
 *   key     - Key pointer.
 *   key_len - Length of the key in bytes.
 *
 * Returns:
 *   Pointer to the stored value, or NULL if the key is not found.
 */
void *dictGet(Dictionary *dict, void *key, size_t key_len);


/**
 * dictFree:
 *   Frees a dictionary and all of its contents.
 *
 * Parameters:
 *   dict - Target dictionary.
 */
void dictFree(Dictionary *dict);


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


/*
 * This hash function is based on the DJB2 hash function
 * Author: Daniel J. Bernstein (djb)
 */
static uint32_t __dictHashBytes(const void *key, size_t key_len, uint32_t size) {
    const uint8_t *data = (const uint8_t *)key;
    uint32_t hash = 5381;
    for (size_t i = 0; i < key_len; i++) {
        hash = ((hash << 5) + hash) ^ data[i]; // hash * 33 ^ data[i]
    }
    return hash % size;
}

static bool __dictIsKeyEqual(const void *k1, size_t l1, const void *k2, size_t l2) {
    if (l1 != l2) return false;
    return (memcmp(k1, k2, l1) == 0);
}

Dictionary *dictNew(uint32_t size, void (*free_key)(void *), void (*free_val)(void *)) {
    Dictionary *d = malloc(sizeof(Dictionary));
    if (!d) return NULL;

    d->size = size;
    d->free_key = free_key;
    d->free_val = free_val;

    d->entries = calloc(size, sizeof(DictEntry *));
    if (!d->entries) {
        free(d);
        return NULL;
    }

    return d;
}

void dictSet(Dictionary *dict, void *key, size_t key_len, void *val) {
    if (!dict || !key) return;

    uint32_t idx = __dictHashBytes(key, key_len, dict->size);
    DictEntry *entry = dict->entries[idx];

    while (entry) {
        if (__dictIsKeyEqual(entry->key, entry->key_len, key, key_len)) {
            if (dict->free_val) dict->free_val(entry->val);
            entry->val = val;
            return;
        }
        entry = entry->next;
    }

    DictEntry *new = malloc(sizeof(DictEntry));
    if (!new) return;


    new->key = (void*)key;
    new->key_len = key_len;
    new->val = val;
    new->next = dict->entries[idx];
    dict->entries[idx] = new;
}

void *dictGet(Dictionary *dict, void *key, size_t key_len) {
    if (!dict || !key) return NULL;

    uint32_t idx = __dictHashBytes(key, key_len, dict->size);
    DictEntry *entry = dict->entries[idx];

    while (entry) {
        if (__dictIsKeyEqual(entry->key, entry->key_len, key, key_len)) {
            return entry->val;
        }
        entry = entry->next;
    }

    return NULL;
}

void dictFree(Dictionary *dict) {
    if (!dict) return;

    for (uint32_t i = 0; i < dict->size; i++) {
        DictEntry *entry = dict->entries[i];
        while (entry) {
            DictEntry *next = entry->next;
            if (dict->free_key) dict->free_key(entry->key);
            if (dict->free_val) dict->free_val(entry->val);
            free(entry);
            entry = next;
        }
    }

    free(dict->entries);
    free(dict);
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