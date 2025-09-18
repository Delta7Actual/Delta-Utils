#include <stdio.h>
#include <stdbool.h>


/* =====================================================================
 *
 * INCLUDED FUNCTIONS
 * 
 * =====================================================================
 */


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


/* =====================================================================
 *
 * IMPLEMENTATION
 * 
 * =====================================================================
 */


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