#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define DA_STRINGS
#define DA_BASE_64
#define DA_VECTOR

#include "../deltautils.h"

int main(void) {
    char s1[] = "   Hello World!  ";
    char s2[] = "foo bar foo";
    char s4[] = "abcabcabc";
    char *tmp;

    // Trim tests
    printf("Original: '%s'\n", s1);
    printf("Trim: '%s'\n", strTrim(strDup(s1)));
    printf("TrimL: '%s'\n", strTrimL(strDup(s1)));
    printf("TrimR: '%s'\n", strTrimR(strDup(s1)));

    // Replace test
    tmp = strReplace(s2, "foo", "baz");
    printf("Replace: '%s'\n", tmp);
    free(tmp);

    // ToLower / ToUpper
    tmp = strDup("AbC123");
    printf("ToLower: '%s'\n", strToLower(tmp));
    free(tmp);

    tmp = strDup("AbC123");
    printf("ToUpper: '%s'\n", strToUpper(tmp));
    free(tmp);

    // StartsWith / EndsWith
    printf("StartsWith 'Hello'?: %d\n", strStartsWith("Hello World", "Hello"));
    printf("EndsWith 'World'?: %d\n", strEndsWith("Hello World", "World"));

    // Slice
    tmp = strSlice("Hello World", 0, 5);
    printf("Slice 0-5: '%s'\n", tmp);
    free(tmp);

    tmp = strSlice("Hello World", 6, 11);
    printf("Slice 6-11: '%s'\n", tmp);
    free(tmp);

    // Count
    printf("Count 'abc' in '%s': %zu\n", s4, strCount(s4, "abc"));

    // Dup
    tmp = strDup("duplicate me");
    printf("Dup: '%s'\n", tmp);
    free(tmp);

    // Reverse
    tmp = strDup("abcd");
    printf("Reverse: '%s'\n", strRev(tmp));
    free(tmp);

    return 0;
}