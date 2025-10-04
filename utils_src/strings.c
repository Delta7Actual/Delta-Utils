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


#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// In order to use this functionality you must define DU_VECTOR
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Splits the string `s` by delimiter `delim`.
// Returns a NULL-terminated array of strings; caller must free each element and the array.
char **strSplit(const char *s, const char delim);

// Joins an array of strings `parts` into a single string, using `sep` as separator.
// Returns a newly allocated string; caller must free.
char *strJoin(const char **parts, const char *sep);


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// In order to use this functionality you must define DU_VECTOR
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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