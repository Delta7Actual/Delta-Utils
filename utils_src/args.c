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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
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


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


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