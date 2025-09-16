#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define DA_STRINGS
#include "../deltautils.h"


int main(void) {

    const char *test = "one,two,three,four,five";
    char **parts = strSplit(test, ',');

    printf("Parts:\n");
    for (uint32_t i = 0; i < vecLength(parts); i++) {
        printf("%s\n", *(char **)vecAt(parts, i));
    }

    char *back = strJoin((const char **)parts, " | ");
    printf("And now rejoined: %s\n", back);

    free(back);
    vecFree(parts, free);
    return 0;
}