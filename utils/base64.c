#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

// Hello -> [Hel, lo=] -> SGVsbG8=
size_t b64encode(__uint8_t *in, size_t len, char *out) {
    if (!in || !out || len == 0) return 0;

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
    if (!in || !out || len == 0 || len % 4 != 0) return 0;

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

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Must have 1 input!\n");
        return 1;
    }
    
    size_t input_len = strlen(argv[1]);

    // Encode
    size_t encoded_len = ((input_len + 2) / 3) * 4;
    char *encoded = malloc(encoded_len + 1);
    b64encode((__uint8_t *)argv[1], input_len, encoded);
    encoded[encoded_len] = '\0';
    printf("Encoded: %s\n", encoded);

    // Decode
    __uint8_t *decoded = malloc(input_len + 1);
    size_t decoded_len = b64decode(encoded, encoded_len, decoded);
    decoded[decoded_len] = '\0';
    printf("Decoded: %s\n", decoded);

    free(encoded);
    free(decoded);
    return 0;
}