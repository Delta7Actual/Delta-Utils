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


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


// Encoding table
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

// Decoding table
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