/* =====================================================================
 *
 * "deltautils.h" - General-purpose utility library for C
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
 * -   DU_HASH    | Hashing and digesting
 * -   DU_VECTOR  | Vector operations
 * -   DU_DICT    | Hashmap operations
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
/* =====================================================================
 *
 * BASE64 ENCODING AND DECODING
 *
 * =====================================================================
 */


/**
 * b64Encode:
 *   Encodes binary data into Base64.
 *
 * Parameters:
 *   in  - pointer to input byte array
 *   len - length of input in bytes
 *   out - output buffer (must be large enough to hold encoded string)
 *
 * Returns:
 *   Number of bytes written to 'out'
 */
size_t b64Encode(uint8_t *in, size_t len, char *out);

/**
 * b64Decode:
 *   Decodes a Base64-encoded string into binary data.
 *
 * Parameters:
 *   in  - pointer to Base64 string
 *   len - length of input string
 *   out - output buffer for decoded bytes
 *
 * Returns:
 *   Number of bytes written to 'out'
 */
size_t b64Decode(char *in, size_t len, uint8_t *out);

#endif // DU_BASE64


#ifdef DU_HASH
/* =====================================================================
 *
 * HASH UTILITIES
 *
 * =====================================================================
 */


/**
 * md5Digest:
 *   Computes the MD5 digest of a data block.
 *
 * Parameters:
 *   data - pointer to input data
 *   len  - length of input data in bytes
 *   out  - output buffer of 16 bytes to hold the MD5 digest
 *
 * Returns:
 *   void (result stored in `out`)
 */
void md5Digest(uint8_t *data, size_t len, uint8_t out[16]);

#endif // DU_HASH


#ifdef DU_VECTOR
/* =====================================================================
 *
 * VECTOR OPERATIONS
 *
 * =====================================================================
 */


/**
 * Vector:
 *   Generic dynamic array structure, storing metadata about allocated
 *   capacity, element size, and current length.
 */
typedef struct vec_meta_s {
    uint32_t  capacity;  // How many cells can the vector hold before resizing
    uint32_t    length;  // The length of the vector
    uint16_t cell_size;  // The size of each cell in bytes
    void *        data;  // A pointer to the data stored in the vector
} Vector;

/**
 * vecNew:
 *   Allocates and initializes a new vector.
 *
 * Parameters:
 *   cell_size    - Size of each element in bytes (must be > 0)
 *   capacity_opt - Initial capacity (0 = default of 4)
 *   do_clear     - Whether to zero-initialize allocated memory
 *
 * Returns:
 *   Pointer to the newly allocated vector, or NULL if allocation fails
 */
Vector *vecNew(uint16_t cell_size, uint32_t capacity_opt, bool do_clear);


/**
 * vecFree:
 *   Frees a vector and its internal data buffer.
 *
 * Parameters:
 *   vec        - Pointer to vector
 *   purge_data - If true, zeroes out the vector's internal data before freeing
 */
void vecFree(Vector *vec, bool purge_data);


/**
 * vecLength:
 *   Returns the number of elements currently stored in the vector.
 *
 * Parameters:
 *   vec - Vector pointer
 *
 * Returns:
 *   Current length of the vector
 */
uint32_t vecLength(const Vector *vec);


/**
 * vecCellSize:
 *   Returns the size in bytes of each element in the vector.
 *
 * Parameters:
 *   vec - Vector pointer
 *
 * Returns:
 *   Size of one element in bytes
 */
uint16_t vecCellSize(const Vector *vec);


/**
 * vecCapacity:
 *   Returns the current allocated capacity of the vector.
 *
 * Parameters:
 *   vec - Vector pointer
 *
 * Returns:
 *   Number of elements the vector can hold before resizing
 */
uint32_t vecCapacity(const Vector *vec);


/**
 * vecSet:
 *   Sets the value of an element at a specific index.
 *
 * Parameters:
 *   vec - Vector pointer
 *   idx - Index of the element (must be < vecLength(vec))
 *   val - Pointer to the new value to store
 */
void vecSet(Vector *vec, uint32_t idx, const void *val);


/**
 * vecAt:
 *   Returns a pointer to the element at the specified index.
 *
 * Parameters:
 *   vec - Vector pointer
 *   idx - Index of the element (must be < vecLength(vec))
 *
 * Returns:
 *   Pointer to the element inside the vector
 */
void *vecAt(Vector *vec, uint32_t idx);


/**
 * vecPush:
 *   Appends a new element to the end of the vector, resizing if needed.
 *
 * Parameters:
 *   vec - Vector pointer
 *   val - Pointer to the value to append
 */
void vecPush(Vector *vec, const void *val);


/**
 * vecPop:
 *   Removes and returns the last element from the vector.
 *
 * Parameters:
 *   vec - Vector pointer (must have at least 1 element)
 *
 * Returns:
 *   Pointer to the popped element inside the buffer.
 *   NOTE: This pointer becomes invalid if the vector is resized afterward.
 */
void *vecPop(Vector *vec);


/**
 * vecReserve:
 *   Ensures the vector has at least 'new_capacity' slots allocated.
 *
 * Parameters:
 *   vec          - Vector pointer
 *   new_capacity - Desired minimum capacity
 *   do_clear     - If true, zero-initializes newly allocated space
 */
void vecReserve(Vector *vec, uint32_t new_capacity, bool do_clear);

#endif // DU_VECTOR


#ifdef DU_DICT
/* =====================================================================
 *
 * HASHMAP OPERATIONS
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

#endif // DU_DICT


#ifdef DU_ARGS
/* =====================================================================
 *
 * CLI ARGUMENT PARSING
 *
 * =====================================================================
 */


/**
 * ArgType:
 *   Enumerates the supported argument types.
 */
typedef enum {
    DU_ARG_BOL,   // Boolean flag (true if present)
    DU_ARG_INT,   // Integer argument
    DU_ARG_DBL,   // Double/float argument
    DU_ARG_STR,   // String argument
    DU_ARG_END    // Marks the end of an ArgSpec array
} ArgType;

/**
 * ArgSpec:
 *   Describes one command-line argument option.
 */
typedef struct {
    const char *s_rep;   // Short option name (e.g., "h") or NULL
    const char *l_rep;   // Long option name (e.g., "help") or NULL
    const char *help;    // Help text (can be NULL)
    void       *out;     // Pointer to storage for the parsed value
    ArgType     type;    // Argument type
    bool        is_req;  // Whether this argument is required
} ArgSpec;

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


/**
 * parseArgs:
 *   Parses command-line arguments according to the given specification array.
 *
 * Parameters:
 *   ctx   - Pointer to an ArgSpec array describing expected arguments.
 *           Must be terminated with ARG_END().
 *   argc  - Argument count from main().
 *   argv  - Argument vector from main().
 *
 *
 * Returns:
 *   true  if parsing succeeded with all requirements met.
 *   false if an error occurred.
 */
bool parseArgs(ArgSpec *ctx, int argc, char **argv);


/**
 * printHelp:
 *   Prints usage information for the program based on the argument
 *   specification array.
 *
 * Parameters:
 *   progName - Name of the program (usually argv[0]).
 *   ctx      - Pointer to an ArgSpec array (must be terminated with ARG_END()).
 */
void printHelp(const char *progName, ArgSpec *ctx);

#endif // DU_ARGS


#ifdef DU_STRINGS

/* =====================================================================
 *
 * STRING UTILITIES
 *
 * =====================================================================
 */


/**
 * strTrim:
 *   Removes whitespace from both ends of the string in-place.
 *
 * Parameters:
 *   s - string to trim
 *
 * Returns:
 *   Pointer to the trimmed string (same as input)
 */
char *strTrim(char *s);


/**
 * strLTrim:
 *   Removes whitespace from the start of the string in-place.
 *
 * Parameters:
 *   s - string to trim
 *
 * Returns:
 *   Pointer to the trimmed string (same as input)
 */
char *strLTrim(char *s);


/**
 * strRTrim:
 *   Removes whitespace from the end of the string in-place.
 *
 * Parameters:
 *   s - string to trim
 *
 * Returns:
 *   Pointer to the trimmed string (same as input)
 */
char *strRTrim(char *s);


/**
 * strReplace:
 *   Replaces all occurrences of `needle` in `s` with `replacement`.
 *
 * Parameters:
 *   s           - input string
 *   needle      - substring to replace
 *   replacement - string to insert
 *
 * Returns:
 *   Newly allocated string; caller must free
 */
char *strReplace(const char *s, const char *needle, const char *replacement);


/**
 * strToLower:
 *   Converts the string to lowercase in-place.
 *
 * Parameters:
 *   s - string to convert
 *
 * Returns:
 *   Pointer to the converted string (same as input)
 */
char *strToLower(char *s);


/**
 * strToUpper:
 *   Converts the string to uppercase in-place.
 *
 * Parameters:
 *   s - string to convert
 *
 * Returns:
 *   Pointer to the converted string (same as input)
 */
char *strToUpper(char *s);


/**
 * strStartsWith:
 *   Checks if the string starts with the given prefix.
 *
 * Parameters:
 *   s      - string to check
 *   prefix - prefix to test
 *
 * Returns:
 *   true if `s` starts with `prefix`, false otherwise
 */
bool strStartsWith(const char *s, const char *prefix);


/**
 * strEndsWith:
 *   Checks if the string ends with the given suffix.
 *
 * Parameters:
 *   s      - string to check
 *   suffix - suffix to test
 *
 * Returns:
 *   true if `s` ends with `suffix`, false otherwise
 */
bool strEndsWith(const char *s, const char *suffix);


/**
 * strSlice:
 *   Returns a newly allocated substring from `start` to `end` (exclusive).
 *   Negative indices count from the end of the string.
 *
 * Parameters:
 *   s     - input string
 *   start - starting index
 *   end   - ending index (exclusive)
 *
 * Returns:
 *   Newly allocated substring; caller must free
 */
char *strSlice(const char *s, size_t start, size_t end);


/**
 * strCount:
 *   Counts the number of occurrences of `needle` in `s`.
 *
 * Parameters:
 *   s      - string to search
 *   needle - substring to count
 *
 * Returns:
 *   Number of occurrences
 */
size_t strCount(const char *s, const char *needle);


/**
 * strDup:
 *   Returns a newly allocated copy of the string.
 *
 * Parameters:
 *   s - string to duplicate
 *
 * Returns:
 *   Newly allocated string; caller must free
 */
char *strDup(const char *s);


/**
 * strRev:
 *   Reverses the string in-place.
 *
 * Parameters:
 *   s - string to reverse
 *
 * Returns:
 *   Pointer to the reversed string (same as input)
 */
char *strRev(char *s);


#ifdef DU_VECTOR
/*
*/

/**
 * strSplit:
 *   Splits the string `s` by delimiter `delim`.
 *
 * Parameters:
 *   s     - string to split
 *   delim - delimiter character
 *
 * Returns:
 *   NULL-terminated array of strings; caller must free each element and the array
 */
char **strSplit(const char *s, const char delim);


/**
 * strJoin:
 *   Joins an array of strings `parts` into a single string, using `sep` as separator.
 *
 * Parameters:
 *   parts - array of strings to join
 *   sep   - separator string
 *
 * Returns:
 *   Newly allocated string; caller must free
 */
char *strJoin(const char **parts, const char *sep);

#endif // DU_VECTOR

#endif // DU_STRINGS


#ifdef DU_TUI
/* =====================================================================
 *
 * TERMINAL UI UTILITIES
 *
 * =====================================================================
 */


#define DU_TUI_RESET       "\x1b[0m"
#define DU_TUI_BOLD        "\x1b[1m"
#define DU_TUI_DIM         "\x1b[2m"
#define DU_TUI_UNDERLINE   "\x1b[4m"
#define DU_TUI_BLINK       "\x1b[5m"
#define DU_TUI_REVERSE     "\x1b[7m"
#define DU_TUI_HIDDEN      "\x1b[8m"

#define DU_TUI_FG_BLACK    "\x1b[30m"
#define DU_TUI_FG_RED      "\x1b[31m"
#define DU_TUI_FG_GREEN    "\x1b[32m"
#define DU_TUI_FG_YELLOW   "\x1b[33m"
#define DU_TUI_FG_BLUE     "\x1b[34m"
#define DU_TUI_FG_MAGENTA  "\x1b[35m"
#define DU_TUI_FG_CYAN     "\x1b[36m"
#define DU_TUI_FG_WHITE    "\x1b[37m"

#define DU_TUI_FG_BBLACK   "\x1b[90m"
#define DU_TUI_FG_BRED     "\x1b[91m"
#define DU_TUI_FG_BGREEN   "\x1b[92m"
#define DU_TUI_FG_BYELLOW  "\x1b[93m"
#define DU_TUI_FG_BBLUE    "\x1b[94m"
#define DU_TUI_FG_BMAGENTA "\x1b[95m"
#define DU_TUI_FG_BCYAN    "\x1b[96m"
#define DU_TUI_FG_BWHITE   "\x1b[97m"

#define DU_TUI_BG_BLACK    "\x1b[40m"
#define DU_TUI_BG_RED      "\x1b[41m"
#define DU_TUI_BG_GREEN    "\x1b[42m"
#define DU_TUI_BG_YELLOW   "\x1b[43m"
#define DU_TUI_BG_BLUE     "\x1b[44m"
#define DU_TUI_BG_MAGENTA  "\x1b[45m"
#define DU_TUI_BG_CYAN     "\x1b[46m"
#define DU_TUI_BG_WHITE    "\x1b[47m"

#define DU_TUI_BG_BBLACK   "\x1b[100m"
#define DU_TUI_BG_BRED     "\x1b[101m"
#define DU_TUI_BG_BGREEN   "\x1b[102m"
#define DU_TUI_BG_BYELLOW  "\x1b[103m"
#define DU_TUI_BG_BBLUE    "\x1b[104m"
#define DU_TUI_BG_BMAGENTA "\x1b[105m"
#define DU_TUI_BG_BCYAN    "\x1b[106m"
#define DU_TUI_BG_BWHITE   "\x1b[107m"


/**
 * tuiSetColor:
 *   Sets the foreground and/or background color for text output.
 *
 * Parameters:
 *   foreground - ANSI escape code string for foreground color (e.g., DU_TUI_RED)
 *   background - ANSI escape code string for background color
 */
void tuiSetColor(const char *foreground, const char *background);

/**
 * tuiReset:
 *   Resets text formatting and colors to defaults.
 */
void tuiReset(void);

/**
 * tuiCursorUp:
 *   Moves the cursor up by `n` lines.
 *
 * Parameters:
 *   n - number of lines to move up
 */
void tuiCursorUp(uint8_t n);

/**
 * tuiCursorDown:
 *   Moves the cursor down by `n` lines.
 *
 * Parameters:
 *   n - number of lines to move down
 */
void tuiCursorDown(uint8_t n);

/**
 * tuiCursorRight:
 *   Moves the cursor right by `n` columns.
 *
 * Parameters:
 *   n - number of columns to move right
 */
void tuiCursorRight(uint8_t n);

/**
 * tuiCursorLeft:
 *   Moves the cursor left by `n` columns.
 *
 * Parameters:
 *   n - number of columns to move left
 */
void tuiCursorLeft(uint8_t n);

/**
 * tuiCursorPos:
 *   Moves the cursor to absolute position (x, y).
 *
 * Parameters:
 *   x - column number (1-based)
 *   y - row number (1-based)
 */
void tuiCursorPos(uint8_t x, uint8_t y);

/**
 * tuiHideCursor:
 *   Hides the terminal cursor.
 */
void tuiHideCursor(void);

/**
 * tuiShowCursor:
 *   Shows the terminal cursor if it was hidden.
 */
void tuiShowCursor(void);

/**
 * tuiClearScreen:
 *   Clears the terminal screen and moves cursor to top-left.
 */
void tuiClearScreen(void);

#endif // DU_TUI


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


#ifdef DU_HASH

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

#endif // DU_HASH


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


#ifdef DU_DICT

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

#endif // DU_DICT


#ifdef DU_ARGS

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
    else if (l_rep) {
        if (strcmp(argument, l_rep) == 0) {
            return true;
        }
    }

    return false;
}

bool parseArgs(ArgSpec *ctx, int argc, char **argv) {
    if (!ctx || argc == 1) return false;

    bool isGood = true;

    int count = 0;
    for (ArgSpec *spec = ctx; spec->type != DU_ARG_END; spec++) count++;
    bool *found = calloc(count, sizeof(bool));

    for (int i = 1; i < argc; i++) {
        char *argument = argv[i];
        int idx = 0;

        for (ArgSpec *spec = ctx; spec->type != DU_ARG_END; spec++, idx++) {
            if (__isFlagDec(argument, spec->s_rep, spec->l_rep)) {
                found[idx] = true;

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
                            isGood = false;
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
                            isGood = false;
                        }
                        break;
                    }
                
                    case DU_ARG_STR: {
                        if (!spec->out) break;
                        if (i + 1 < argc) {
                            *(char **)spec->out = argv[++i];
                        } else {
                            fprintf(stderr, "Error: expected string value after %s\n", argument);
                            isGood = false;
                        }
                        break;
                    }

                    // Just to shut up the compiler warnings
                    case DU_ARG_END: {
                        break;
                    }
                }
            }
        }
    }

    int idx = 0;
    for (ArgSpec *spec = ctx; spec->type != DU_ARG_END; spec++, idx++) {
        if (spec->is_req && !found[idx]) {
            isGood = false;
            fprintf(stderr, "Error: required argument --%s (or -%s) missing\n",
                    spec->l_rep ? spec->l_rep : "",
                    spec->s_rep ? spec->s_rep : "");
        }
    }

    free(found);
    return isGood;
}

void printHelp(const char *prog, ArgSpec *ctx) {
    if (!ctx) return;

    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n");


    for (ArgSpec *spec = ctx; spec->type != DU_ARG_END; spec++) {
        // Build option string
        char optBuf[128] = {0};

        switch (spec->type) {
            case DU_ARG_INT:
                strncat(optBuf, " <int>   ", sizeof(optBuf) - strlen(optBuf) - 1);
                break;
            case DU_ARG_DBL:
                strncat(optBuf, " <double>", sizeof(optBuf) - strlen(optBuf) - 1);
                break;
            case DU_ARG_STR:
                strncat(optBuf, " <string>", sizeof(optBuf) - strlen(optBuf) - 1);
                break;
            default:
                break;
        }

        if (spec->s_rep) {
            snprintf(optBuf + strlen(optBuf), sizeof(optBuf) - strlen(optBuf),
                        "-%s", spec->s_rep);
        }

        if (spec->l_rep) {
            if (strlen(optBuf) > 0) {
                strncat(optBuf, ", ", sizeof(optBuf) - strlen(optBuf) - 1);
            }
            snprintf(optBuf + strlen(optBuf), sizeof(optBuf) - strlen(optBuf),
                        "--%s", spec->l_rep);
        }

        printf("  %-20s %s%s\n",
                optBuf,
                spec->help ? spec->help : "",
                spec->is_req ? " (required)" : "");
    }

    printf("\n");
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

void tuiSetColor(const char *foreground, const char *background) {
    if (foreground) printf("%s", foreground);
    if (background) printf("%s", background);
}

void tuiReset(void) {
    printf("%s", DU_TUI_RESET);
}

void tuiCursorUp(uint8_t n) {
    if (n > 0) printf("\x1b[%uA", n);
}

void tuiCursorDown(uint8_t n) {
    if (n > 0) printf("\x1b[%uB", n);
}

void tuiCursorRight(uint8_t n) {
    if (n > 0) printf("\x1b[%uC", n);
}

void tuiCursorLeft(uint8_t n) {
    if (n > 0) printf("\x1b[%uD", n);
}

void tuiCursorPos(uint8_t x, uint8_t y) {
    printf("\x1b[%u;%uH", y, x);
}

void tuiHideCursor(void) {
    printf("\x1b[?25l");
}

void tuiShowCursor(void) {
    printf("\x1b[?25h");
}

void tuiClearScreen(void) {
    printf("\x1b[2J\x1b[H");
}

#endif // DU_TUI


#ifdef __cplusplus
}
#endif


#endif // DELTAUTILS_H