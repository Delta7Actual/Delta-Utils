#ifndef DUTILITIES_H
#define DUTILITIES_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

// Declerations

size_t b64decode(char *in, size_t len, uint8_t *out);
size_t b64encode(uint8_t *in, size_t len, char *out);

void md5_digest(uint8_t *data, size_t len, uint8_t out[16]);

#ifdef DA_BASE64

static const __uint8_t b64_dt[256] = {
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

size_t b64encode(__uint8_t *in, size_t len, char *out) {
    assert(in != NULL && len != 0 && out != NULL);

    size_t idx = 0;
    int i = 0;
    while (i+2 < len) {
        __uint32_t g = (in[i] << 16) | (in[i+1] << 8) | in[i+2];
        i += 3;

        out[idx++] = b64_et[(g >> 18) & 0b111111];
        out[idx++] = b64_et[(g >> 12) & 0b111111];
        out[idx++] = b64_et[(g >> 6 ) & 0b111111];
        out[idx++] = b64_et[g & 0b111111];
    }

    size_t rem = len - i;

    if (rem == 1) {
        __uint32_t g = in[i] << 16;
        out[idx++] = b64_et[(g >> 18) & 0b111111];
        out[idx++] = b64_et[(g >> 12) & 0b111111];
        out[idx++] = '=';
        out[idx++] = '=';
    }
    if (rem == 2) {
        __uint32_t g = (in[i] << 16) | (in[i+1] << 8);
        out[idx++] = b64_et[(g >> 18) & 0b111111];
        out[idx++] = b64_et[(g >> 12) & 0b111111];
        out[idx++] = b64_et[(g >> 6 ) & 0b111111];
        out[idx++] = '=';
    }

    return idx;
}

size_t b64decode(char *in, size_t len, __uint8_t *out) {
    assert(in != NULL && len != 0 && out != NULL);

    size_t idx = 0;
    for (size_t i = 0; i < len; i += 4) {
        __uint8_t c1 = in[ i ] == '=' ? 0 : b64_dt[(__uint8_t)in[ i ]];
        __uint8_t c2 = in[i+1] == '=' ? 0 : b64_dt[(__uint8_t)in[i+1]];
        __uint8_t c3 = in[i+2] == '=' ? 0 : b64_dt[(__uint8_t)in[i+2]];
        __uint8_t c4 = in[i+3] == '=' ? 0 : b64_dt[(__uint8_t)in[i+3]];

        __uint32_t g = (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;

        out[idx++] = (g >> 16) & 0b11111111;

        if (in[i+2] != '=') out[idx++] = (g >> 8) & 0b11111111;
        if (in[i+3] != '=') out[idx++] = g & 0b11111111;
    }

    return idx;
}

#endif DA_BASE64

#ifdef DA_MD5

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

void md5_init(Context *ctx) {
    assert(ctx != NULL);
    memset(ctx, 0, sizeof(Context));
    ctx->a = AI;
    ctx->b = BI;
    ctx->c = CI;
    ctx->d = DI;
}

void md5_handle_block(Context *ctx, uint8_t block[64]) {
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

void md5_update_len(Context *ctx, uint8_t *in, size_t len, int update_len) {
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
            md5_handle_block(ctx, ctx->buff);
            ctx->blen = 0;
        }
    }

    while (len - offset >= 64) {
        md5_handle_block(ctx, in + offset);
        offset += 64;
    }

    if (len > offset) {
        ctx->blen = len - offset;
        memcpy(ctx->buff, in + offset, ctx->blen);
    }
}

void md5_update(Context *ctx, uint8_t *in, size_t len) {
    md5_update_len(ctx, in, len, 1);
}

void md5_finalize(Context *ctx, char out[16]) {
    assert(ctx != NULL && out != NULL);

    uint8_t padding[64] = {0x80};
    size_t pad_len = (ctx->blen < 56) ? (56 - ctx->blen) : (120 - ctx->blen);
    md5_update_len(ctx, padding, pad_len, 0); 

    uint8_t length_bytes[8];
    uint64_t size_bits = ctx->size * 8;
    for (int i = 0; i < 8; i++) {
        length_bytes[i] = (uint8_t)((size_bits >> (8 * i)) & 0xFF);
    }
    md5_update_len(ctx, length_bytes, 8, 0);

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

void md5_digest(uint8_t *data, size_t len, uint8_t out[16]) {
    Context ctx = {0};
    md5_init(&ctx);
    md5_update(&ctx, data, len);
    md5_finalize(&ctx, (char *)out);
}

#endif // DA_MD5

#ifdef DA_VECTOR

typedef struct vec_meta_s {
    uint32_t capacity;
    uint32_t length;
    uint16_t cell_size;
} VecMeta;

#define __VEC_GET_META(d) ((VecMeta *)((char *)(d) - sizeof(VecMeta)))
#define __VEC_GET_DATA(v) ((void *)((char *)(v) + sizeof(VecMeta)))

void *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear);
void vecFree(void *data, bool do_purge);
void __vecPurge(VecMeta *v);
void vecSet(void *data, uint32_t idx, void *val);
void *vecAt(void *data, uint32_t idx);

#define vecPush(d, val) ((d) = __vecPush((d), (val)))
void *__vecPush(void *data, void *val);

void *vecPop(void *data);

#define vecReserve(d, cap, clr) ((d) = __vecReserve((d), (cap), (clr)))
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

#endif // DA_VECTOR

#endif