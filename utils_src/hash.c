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


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


// Computes the MD5 digest of a data block.
// Parameters:
//   data - input data
//   len  - length of input data
//   out  - output buffer of 16 bytes to hold MD5 digest
void md5Digest(uint8_t *data, size_t len, uint8_t out[16]);


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


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