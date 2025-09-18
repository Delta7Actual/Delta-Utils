/* =====================================================================
 *
 * "deltautils.h" — version 1.0.0
 * General-purpose utility library for C
 *
 * Source Code: https://github.com/Delta7Actual/Delta-Utils
 * Created and maintained by Dror Sheffer
 *
 * Licensed under the MIT License.
 * See the accompanying LICENSE file for full terms.
 *
 * =====================================================================
 *
 * HOW TO USE:
 *
 * This library is modular. Define the modules you need before
 * including the header file. Like so:
 *
 *     #define DU_EXAMPLE
 *     #include "deltautils.h"
 *
 * You can include multiple modules by defining them one after another
 * before including the header:
 *
 *     #define DU_BASE64
 *     #define DU_VECTOR
 *     #define DU_STRINGS
 *     #include "deltautils.h"
 *
 * =====================================================================
 * 
 * MODULE LIST:
 * 
 * -   DU_BASE64  | base64 Encoding and decoding
 * -   DU_MD5     | md5 Hashing and digesting
 * -   DU_VECTOR  | Vector operations
 * -   DU_HASHMAP | Hashmap operations
 * -   DU_ARGS    | CLI argument handling
 * -   DU_STRINGS | Expanded string operations
 * -   DU_TUI     | Terminal UI functionality
 * 
 * =====================================================================
 */


#ifndef DELTAUTILS_H
#define DELTAUTILS_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


/* =====================================================================
 *
 * Delta-Utils: DECLARATIONS AND DEFINITIONS
 * 
 * =====================================================================
 */


#ifdef DU_BASE64

// Encodes binary data into Base64.
// Parameters:
//   in  - input byte array
//   len - length of input
//   out - output buffer (must be large enough)
// Returns: number of bytes written to 'out'
size_t b64Encode(uint8_t *in, size_t len, char *out);

// Decodes Base64 string into binary data.
// Parameters:
//   in  - Base64 encoded string
//   len - length of input string
//   out - output buffer
// Returns: number of bytes written to 'out'
size_t b64Decode(char *in, size_t len, uint8_t *out);

#endif // DU_BASE64


#ifdef DU_MD5

// Computes the MD5 digest of a data block.
// Parameters:
//   data - input data
//   len  - length of input data
//   out  - output buffer of 16 bytes to hold MD5 digest
void md5Digest(uint8_t *data, size_t len, uint8_t out[16]);

#endif // DU_MD5


#ifdef DU_VECTOR

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

#endif // DU_VECTOR


#ifdef DU_ARGS

// For the following macros:
// - sr   = short option (e.g., "h")
// - lr   = long option (e.g., "help")
// - o    = pointer to storage for the value
// - h    = help description
// - ir   = whether the argument is required

// ARG_BOOL: Define a boolean flag argument.
#define ARG_BOOL(sr, lr, o, h, ir) \
    ((ArgSpec){ (sr), (lr), (h), (o), DU_ARG_BOL, (ir) })

// ARG_INT: Define an integer argument.
#define ARG_INT(sr, lr, o, h, ir) \
    ((ArgSpec){ (sr), (lr), (h), (o), DU_ARG_INT, (ir) })

// ARG_DOUBLE: Define a floating-point argument.
#define ARG_DOUBLE(sr, lr, o, h, ir) \
    ((ArgSpec){ (sr), (lr), (h), (o), DU_ARG_DBL, (ir) })

// ARG_STRING: Define a string argument.
#define ARG_STRING(sr, lr, o, h, ir) \
    ((ArgSpec){ (sr), (lr), (h), (o), DU_ARG_STR, (ir) })

// ARG_END: Marks the end of the argument specification array.
// Must be placed as the last element in ArgSpec array.
#define ARG_END() \
    ((ArgSpec){NULL, NULL, NULL, NULL, DU_ARG_END, false})

// parseArgs: Parses command-line arguments according to the given specification array.
// ctx    = pointer to ArgSpec array describing expected arguments
// argc   = number of command-line arguments
// argv   = array of command-line arguments
// Populates the storage pointers in each ArgSpec as arguments are found.
// May print errors or help messages if required arguments are missing.
void parseArgs(ArgSpec *ctx, int argc, char **argv);

#endif // DU_ARGS


#ifdef DU_STRINGS

// Removes whitespace from both ends of the string in-place.
char *strTrim(char *s);

// Removes whitespace from the start of the string in-place.
char *strLTrim(char *s);

// Removes whitespace from the end of the string in-place.
char *strRTrim(char *s);

// Replaces all occurrences of `needle` in `s` with `replacement`.
// Returns a newly allocated string; caller must free.
char *strReplace(const char *s, const char *needle, const char *replacement);

// Converts the string to lowercase in-place.
char *strToLower(char *s);

// Converts the string to uppercase in-place.
char *strToUpper(char *s);

// Checks if the string starts with the given prefix.
// Returns true if it does, false otherwise.
bool strStartsWith(const char *s, const char *prefix);

// Checks if the string ends with the given suffix.
// Returns true if it does, false otherwise.
bool strEndsWith(const char *s, const char *suffix);

// Returns a newly allocated substring from `start` to `end` (exclusive).
// Negative indices count from the end of the string.
char *strSlice(const char *s, size_t start, size_t end);

// Counts the number of occurrences of `needle` in `s`.
size_t strCount(const char *s, const char *needle);

// Returns a newly allocated copy of the string.
char *strDup(const char *s);

// Reverses the string in-place.
char *strRev(char *s);

#ifdef DU_VECTOR
// --------------------
// In order to use this functionality you must define DU_VECTOR
// --------------------

// Splits the string `s` by delimiter `delim`.
// Returns a NULL-terminated array of strings; caller must free each element and the array.
char **strSplit(const char *s, const char delim);

// Joins an array of strings `parts` into a single string, using `sep` as separator.
// Returns a newly allocated string; caller must free.
char *strJoin(const char **parts, const char *sep);

#endif // DU_VECTOR

#endif // DU_STRINGS


/* =====================================================================
 *
 * Delta-Utils: IMPLEMENTATION
 * 
 * =====================================================================
 */


#ifdef DU_BASE64

static const uint8_t b64_dt[256] = {
    ['A'] =  0, ['B'] =  1, ['C'] =  2, ['D'] =  3,
    ['E'] =  4, ['F'] =  5, ['G'] =  6, ['H'] =  7,
    ['I'] =  8, ['J'] =  9, ['K'] = 10, ['L'] = 11,
    ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
    ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25,
    ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33,
    ['i'] = 34, ['j'] = 35, ['k'] = 36, ['l'] = 37,
    ['m'] = 38, ['n'] = 39, ['o'] = 40, ['p'] = 41,
    ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45,
    ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50, ['z'] = 51,
    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
    ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61,
    ['+'] = 62, ['/'] = 63
};

static const char b64_et[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

size_t b64Encode(uint8_t *in, size_t len, char *out) {
    assert(in != NULL && len != 0 && out != NULL);

    size_t idx = 0;
    int i = 0;
    while (i+2 < len) {
        uint32_t g = (in[i] << 16) | (in[i+1] << 8) | in[i+2];
        i += 3;

        out[idx++] = b64_et[(g >> 18) & 0xF];
        out[idx++] = b64_et[(g >> 12) & 0xF];
        out[idx++] = b64_et[(g >> 6 ) & 0xF];
        out[idx++] = b64_et[g & 0xF];
    }

    size_t rem = len - i;

    if (rem == 1) {
        uint32_t g = in[i] << 16;
        out[idx++] = b64_et[(g >> 18) & 0xF];
        out[idx++] = b64_et[(g >> 12) & 0xF];
        out[idx++] = '=';
        out[idx++] = '=';
    }
    if (rem == 2) {
        uint32_t g = (in[i] << 16) | (in[i+1] << 8);
        out[idx++] = b64_et[(g >> 18) & 0xF];
        out[idx++] = b64_et[(g >> 12) & 0xF];
        out[idx++] = b64_et[(g >> 6 ) & 0xF];
        out[idx++] = '=';
    }

    return idx;
}

size_t b64Decode(char *in, size_t len, uint8_t *out) {
    assert(in != NULL && len != 0 && out != NULL);

    size_t idx = 0;
    for (size_t i = 0; i < len; i += 4) {
        uint8_t c1 = in[ i ] == '=' ? 0 : b64_dt[(uint8_t)in[ i ]];
        uint8_t c2 = in[i+1] == '=' ? 0 : b64_dt[(uint8_t)in[i+1]];
        uint8_t c3 = in[i+2] == '=' ? 0 : b64_dt[(uint8_t)in[i+2]];
        uint8_t c4 = in[i+3] == '=' ? 0 : b64_dt[(uint8_t)in[i+3]];

        uint32_t g = (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;

        out[idx++] = (g >> 16) & 0xFF;

        if (in[i+2] != '=') out[idx++] = (g >> 8) & 0xFF;
        if (in[i+3] != '=') out[idx++] = g & 0xFF;
    }

    return idx;
}

#endif // DU_BASE64

#ifdef DU_MD5

#define AI 0x67452301
#define BI 0xefcdab89
#define CI 0x98badcfe
#define DI 0x10325476

static const uint32_t S[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};

static const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

#define LR32(x,b) (((x) << ((b) & 31)) | ((x) >> (32 - ((b) & 31))))
#define RR32(x,b) (((x) >> ((b) & 31)) | ((x) << (32 - ((b) & 31))))

#define F(x,y,z)  (((x) & (y)) | ((~(x)) & (z)))
#define G(x,y,z)  (((x) & (z)) | ((y) & (~z))  )
#define H(x,y,z)  ((x) ^ (y) ^ (z)             )
#define I(x,y,z)  ((y) ^ ((x) | (~z))          )

#define FF(a, b, c, d, x, s, ac) \
    a += F(b, c, d) + x + ac; \
    a = LR32(a, s); \
    a += b;

#define GG(a, b, c, d, x, s, ac) \
    a += G(b, c, d) + x + ac; \
    a = LR32(a, s); \
    a += b;

#define HH(a, b, c, d, x, s, ac) \
    a += H(b, c, d) + x + ac; \
    a = LR32(a, s); \
    a += b;

#define II(a, b, c, d, x, s, ac) \
    a += I(b, c, d) + x + ac; \
    a = LR32(a, s); \
    a += b;

typedef struct {
    uint32_t a, b, c, d;
    uint8_t    buff[64];
    size_t         size;
    size_t         blen;
} Context;

void __md5Init(Context *ctx) {
    assert(ctx != NULL);
    memset(ctx, 0, sizeof(Context));
    ctx->a = AI;
    ctx->b = BI;
    ctx->c = CI;
    ctx->d = DI;
}

void __md5HandleBlock(Context *ctx, uint8_t block[64]) {
    assert(ctx != NULL && block != NULL);

    uint32_t M[16] = {0};
    for (int i = 0; i < 16; i++) {
        M[i] = (block[i * 4]       ) 
        | (block[(i * 4) + 1] <<  8) 
        | (block[(i * 4) + 2] << 16) 
        | (block[(i * 4) + 3] << 24);
    }

    uint32_t a = ctx->a;
    uint32_t b = ctx->b;
    uint32_t c = ctx->c;
    uint32_t d = ctx->d;

    // Round 1
    FF(a, b, c, d, M[ 0],  7, T[ 0]);
    FF(d, a, b, c, M[ 1], 12, T[ 1]);
    FF(c, d, a, b, M[ 2], 17, T[ 2]);
    FF(b, c, d, a, M[ 3], 22, T[ 3]);
    FF(a, b, c, d, M[ 4],  7, T[ 4]);
    FF(d, a, b, c, M[ 5], 12, T[ 5]);
    FF(c, d, a, b, M[ 6], 17, T[ 6]);
    FF(b, c, d, a, M[ 7], 22, T[ 7]);
    FF(a, b, c, d, M[ 8],  7, T[ 8]);
    FF(d, a, b, c, M[ 9], 12, T[ 9]);
    FF(c, d, a, b, M[10], 17, T[10]);
    FF(b, c, d, a, M[11], 22, T[11]);
    FF(a, b, c, d, M[12],  7, T[12]);
    FF(d, a, b, c, M[13], 12, T[13]);
    FF(c, d, a, b, M[14], 17, T[14]);
    FF(b, c, d, a, M[15], 22, T[15]);

    // Round 2
    GG(a, b, c, d, M[ 1],  5, T[16]);
    GG(d, a, b, c, M[ 6],  9, T[17]);
    GG(c, d, a, b, M[11], 14, T[18]);
    GG(b, c, d, a, M[ 0], 20, T[19]);
    GG(a, b, c, d, M[ 5],  5, T[20]);
    GG(d, a, b, c, M[10],  9, T[21]);
    GG(c, d, a, b, M[15], 14, T[22]);
    GG(b, c, d, a, M[ 4], 20, T[23]);
    GG(a, b, c, d, M[ 9],  5, T[24]);
    GG(d, a, b, c, M[14],  9, T[25]);
    GG(c, d, a, b, M[ 3], 14, T[26]);
    GG(b, c, d, a, M[ 8], 20, T[27]);
    GG(a, b, c, d, M[13],  5, T[28]);
    GG(d, a, b, c, M[ 2],  9, T[29]);
    GG(c, d, a, b, M[ 7], 14, T[30]);
    GG(b, c, d, a, M[12], 20, T[31]);

    // Round 3
    HH(a, b, c, d, M[ 5],  4, T[32]);
    HH(d, a, b, c, M[ 8], 11, T[33]);
    HH(c, d, a, b, M[11], 16, T[34]);
    HH(b, c, d, a, M[14], 23, T[35]);
    HH(a, b, c, d, M[ 1],  4, T[36]);
    HH(d, a, b, c, M[ 4], 11, T[37]);
    HH(c, d, a, b, M[ 7], 16, T[38]);
    HH(b, c, d, a, M[10], 23, T[39]);
    HH(a, b, c, d, M[13],  4, T[40]);
    HH(d, a, b, c, M[ 0], 11, T[41]);
    HH(c, d, a, b, M[ 3], 16, T[42]);
    HH(b, c, d, a, M[ 6], 23, T[43]);
    HH(a, b, c, d, M[ 9],  4, T[44]);
    HH(d, a, b, c, M[12], 11, T[45]);
    HH(c, d, a, b, M[15], 16, T[46]);
    HH(b, c, d, a, M[ 2], 23, T[47]);

    // Round 4
    II(a, b, c, d, M[ 0],  6, T[48]);
    II(d, a, b, c, M[ 7], 10, T[49]);
    II(c, d, a, b, M[14], 15, T[50]);
    II(b, c, d, a, M[ 5], 21, T[51]);
    II(a, b, c, d, M[12],  6, T[52]);
    II(d, a, b, c, M[ 3], 10, T[53]);
    II(c, d, a, b, M[10], 15, T[54]);
    II(b, c, d, a, M[ 1], 21, T[55]);
    II(a, b, c, d, M[ 8],  6, T[56]);
    II(d, a, b, c, M[15], 10, T[57]);
    II(c, d, a, b, M[ 6], 15, T[58]);
    II(b, c, d, a, M[13], 21, T[59]);
    II(a, b, c, d, M[ 4],  6, T[60]);
    II(d, a, b, c, M[11], 10, T[61]);
    II(c, d, a, b, M[ 2], 15, T[62]);
    II(b, c, d, a, M[ 9], 21, T[63]);

    ctx->a += a;
    ctx->b += b;
    ctx->c += c;
    ctx->d += d;

    // Clean up macros
    #undef F
    #undef G
    #undef H
    #undef I
    #undef FF
    #undef GG
    #undef HH
    #undef II
}

void __md5UpdateLen(Context *ctx, uint8_t *in, size_t len, int update_len) {
    assert(ctx != NULL && in != NULL);

    if (update_len) ctx->size += len; 

    size_t offset = 0;
    if (ctx->blen > 0) {
        size_t copy = 64 - ctx->blen;
        if (copy > len) copy = len;

        memcpy(ctx->buff + ctx->blen, in, copy);
        ctx->blen += copy;
        offset += copy;

        if (ctx->blen == 64) {
            __md5HandleBlock(ctx, ctx->buff);
            ctx->blen = 0;
        }
    }

    while (len - offset >= 64) {
        __md5HandleBlock(ctx, in + offset);
        offset += 64;
    }

    if (len > offset) {
        ctx->blen = len - offset;
        memcpy(ctx->buff, in + offset, ctx->blen);
    }
}

void __md5Update(Context *ctx, uint8_t *in, size_t len) {
    __md5UpdateLen(ctx, in, len, 1);
}

void md5_finalize(Context *ctx, char out[16]) {
    assert(ctx != NULL && out != NULL);

    uint8_t padding[64] = {0x80};
    size_t pad_len = (ctx->blen < 56) ? (56 - ctx->blen) : (120 - ctx->blen);
    __md5UpdateLen(ctx, padding, pad_len, 0); 

    uint8_t length_bytes[8];
    uint64_t size_bits = ctx->size * 8;
    for (int i = 0; i < 8; i++) {
        length_bytes[i] = (uint8_t)((size_bits >> (8 * i)) & 0xFF);
    }
    __md5UpdateLen(ctx, length_bytes, 8, 0);

    uint32_t state[4] = { ctx->a, ctx->b, ctx->c, ctx->d };
    for (int i = 0; i < 4; i++) {
        out[(i * 4)    ] = (state[i] >>  0) & 0xFF;
        out[(i * 4) + 1] = (state[i] >>  8) & 0xFF;
        out[(i * 4) + 2] = (state[i] >> 16) & 0xFF;
        out[(i * 4) + 3] = (state[i] >> 24) & 0xFF;
    }

    ctx->blen = 0;
    ctx->size = 0;
    memset(ctx->buff, 0, 64);
}

void md5Digest(uint8_t *data, size_t len, uint8_t out[16]) {
    Context ctx = {0};
    __md5Init(&ctx);
    __md5Update(&ctx, data, len);
    md5_finalize(&ctx, (char *)out);
}

#endif // DU_MD5

#ifdef DU_VECTOR

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

#endif // DU_VECTOR


#ifdef DU_ARGS

typedef enum {
    DU_ARG_BOL, 
    DU_ARG_INT, 
    DU_ARG_DBL, 
    DU_ARG_STR, 
    DU_ARG_END
} ArgType;

typedef struct {
    const char *s_rep;
    const char *l_rep;
    const char  *help;
    void         *out;
    ArgType      type;
    bool       is_req;
} ArgSpec;


static bool __isFlagDec(char *argument, const char *s_rep, const char *l_rep) {
    if (!argument) return false;

    // Check against l_rep
    if (strncmp(argument, "--", 2) == 0 && l_rep) {
        const char *arg_name = argument + 2;
        const char *alias = l_rep;

        while(alias) {
            const char *next = strchr(alias, ',');
            size_t len = (next) ? (size_t)(next - alias) : strlen(alias);

            if (strncmp(alias, arg_name, len) == 0 
                    && (arg_name[len] == '\0' || arg_name[len] == '=')) {
                return true;
            }

            alias = (next) ? next + 1 : NULL;
        }
    }

    // Check against s_rep
    else if (strncmp(argument, "-", 1) == 0 
                && s_rep && argument[1] != '\0' 
                && argument[2] == '\0') {
                    
        char arg_char = *(argument + 1);
        while (*s_rep != '\0') {
            if (arg_char == *s_rep) {
                return true;
            }
            s_rep++;
        }
    }

    // Check whole command
    else {
        if (strcmp(argument, l_rep) == 0) {
            return true;
        }
    }

    return false;
}

void parseArgs(ArgSpec *ctx, int argc, char **argv) {
    if (!ctx || argc == 1) return;

    for (int i = 1; i < argc; i++) {
        char *argument = argv[i];

        for (ArgSpec *spec = ctx; spec->type != DU_ARG_END; spec++) {
            if (__isFlagDec(argument, spec->s_rep, spec->l_rep)) {
                switch (spec->type) {
                    case DU_ARG_BOL:
                        if (spec->out)
                            *(bool *)spec->out = true;
                        break;
                
                    case DU_ARG_INT: {
                        if (!spec->out) break;
                        char *val = strchr(argument, '=');
                        if (val) {
                            *(int *)spec->out = atoi(val + 1);
                        } else if (i + 1 < argc) {
                            *(int *)spec->out = atoi(argv[++i]);
                        } else {
                            fprintf(stderr, "Error: expected integer value after %s\n", argument);
                        }
                        break;
                    }
                
                    case DU_ARG_DBL: {
                        if (!spec->out) break;
                        char *val = strchr(argument, '=');
                        if (val) {
                            *(double *)spec->out = atof(val + 1);
                        } else if (i + 1 < argc) {
                            *(double *)spec->out = atof(argv[++i]);
                        } else {
                            fprintf(stderr, "Error: expected double value after %s\n", argument);
                        }
                        break;
                    }
                
                    case DU_ARG_STR: {
                        if (!spec->out) break;
                        if (i + 1 < argc) {
                            *(char **)spec->out = argv[++i];
                        } else {
                            fprintf(stderr, "Error: expected string value after %s\n", argument);
                        }
                        break;
                    }
                }
            }
        }
    }
}

#endif // DU_ARGS


#ifdef DU_STRINGS

char *strTrimL(char *s) {
    if (!s) return s;

    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;

    memmove(s, p, strlen(p) + 1);
    return s;
}

char *strTrimR(char *s) {
    if (!s) return s;

    char *p = s + strlen(s) - 1;
    while(p >= s && isspace((unsigned char)*p)) p--;

    *(p + 1) = '\0';
    return s;
}

char *strTrim(char *s) {
    if (!s) return s;

    strTrimL(s);
    strTrimR(s);

    return s;
}

char *strReplace(const char *s, const char *needle, const char *replacement) {
    assert(s && needle && replacement);

    size_t n_len = strlen(needle);
    if (n_len == 0) return strDup(s);
    size_t r_len = strlen(replacement);

    size_t new_len = strlen(s) + ((r_len - n_len) * strCount(s, needle));
    
    char *out = malloc(new_len + 1);
    if (!out) return NULL;

    const char *src = s;
    char *dst = out;
    while (*src) {
        if (strncmp(src, needle, n_len) == 0) {
            memcpy(dst, replacement, r_len);
            src += n_len;
            dst += r_len;
        } else {
            *dst++ = *src++;
        }
    }

    *dst = '\0';
    return out;
}

char *strToLower(char *s) {
    if (!s) return s;

    char *p = s;
    while (*p) {
        if (*p >= 'A' && *p <= 'Z') *p += 32;
        p++;
    }

    return s;
}

char *strToUpper(char *s) {
    if (!s) return s;

    char *p = s;
    while (*p) {
        if (*p >= 'a' && *p <= 'z') *p -= 32;
        p++;
    }

    return s;
}

bool strStartsWith(const char *s, const char *prefix) {
    if (!s || !prefix) return false;
    size_t s_len = strlen(s);
    size_t pr_len = strlen(prefix);
    
    if (pr_len > s_len) return false;
    return (strncmp(s, prefix, pr_len) == 0);
}

bool strEndsWith(const char *s, const char *suffix) {
    if (!s || !suffix) return false;
    size_t s_len = strlen(s);
    size_t su_len = strlen(suffix);

    if (su_len > s_len) return false;
    return strncmp(s + s_len - su_len, suffix, su_len) == 0;
}

char *strSlice(const char *s, size_t start, size_t end) {
    assert(s);

    size_t s_len = strlen(s);
    if (start >= s_len || start >= end) return strDup("");
    if (end > s_len) end = s_len;

    size_t len = end - start;
    char *out = malloc(len + 1);

    memcpy(out, s + start, len);
    out[len] = '\0';

    return out;
}

size_t strCount(const char *s, const char *needle) {
    size_t n_len = strlen(needle);
    size_t c = 0;

    const char *p = s;
    while((p = strstr(p, needle))) {
        c++;
        p += n_len;
    }

    return c;
}

char *strDup(const char *s) {
    assert(s);
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

char *strRev(char *s) {
    if (!s || strlen(s) <= 1) return s;

    size_t l = 0, r = strlen(s) - 1;
    while (l < r) {
        char temp = s[l];
        s[l++] = s[r];
        s[r--] = temp;
    }

    return s;
}

#ifdef DU_VECTOR

char **strSplit(const char *s, const char delim) {
    if (!s) return NULL;

    char needle[2] = { delim, '\0' };
    uint32_t p_len = (uint32_t)strCount(s, needle) + 1;
    if (p_len == 1) return NULL;

    char **parts = (char **)vecNew(sizeof(char *), p_len, true);
    if (!parts) return NULL;

    const char *p = s;
    size_t l = 0, r = 0;
    while(*p) {
        if (*p == delim) {
            r = p - s;
            char *tok = strSlice(s, l, r);
            vecPush(parts, &tok);
            l = r + 1;
        }
        p++;
    }

    r = p - s;
    if (r > l) {
        char *tok = strSlice(s, l, r);
        vecPush(parts, &tok);
    } else {
        // trailing delim
        char *tok = strDup("");
        vecPush(parts, &tok);
    }

    return parts;
}

char *strJoin(const char **parts, const char *sep) {
    uint32_t count = vecLength(parts);
    if (count == 0) return NULL;

    size_t sep_len = strlen(sep);
    size_t total_len = 0;
    
    for (uint32_t i = 0; i < count; i++) {
        char *s = *(char **)vecAt(parts, i);
        total_len += strlen(s);
        if (i < count - 1) total_len += sep_len;
    }

    char *joined = malloc(total_len + 1);
    if (!joined) return NULL;

    char *p = joined;
    for (uint32_t i = 0; i < count; i++) {
        char *s = *(char **)vecAt(parts, i);
        size_t len = strlen(s);
        memcpy(p, s, len);
        p += len;

        if (i < count - 1) {
            memcpy(p, sep, sep_len);
            p += sep_len;
        }
    }

    *p = '\0';
    return joined;
}

#endif // DU_VECTOR

#endif // DU_STRINGS


#ifdef DU_TUI

/* Implementation to come. */

#endif // DU_TUI


#ifdef __cplusplus
}
#endif


#endif // DELTAUTILS_H