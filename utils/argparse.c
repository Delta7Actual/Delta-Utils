#define DA_ARGS
#include "../dutilities.h"

int main(int argc, char **argv) {

bool isVerbose = false;
    int maxVal = 0;
    double dVal = 0.0;
    char *outfile = NULL;

    ArgSpec ctx[] = {
        ARG_BOOL("vV", "verbose,verbose_output", &isVerbose, "Enable verbose output", false),
        ARG_INT("m", "max,maximum", &maxVal, "Maximum value allowed", false),
        ARG_DOUBLE(NULL, "dbl", &dVal, "A floating point value", false),
        ARG_STRING("o", "output", &outfile, "Output file name", false),
        ARG_END()
    };

    parseArguments(ctx, argc, argv);

    printf("Results:\n");
    printf("  Verbose: %s\n", isVerbose ? "true" : "false");
    printf("  MaxVal: %d\n", maxVal);
    printf("  Double: %f\n", dVal);
    printf("  Outfile: %s\n", outfile ? outfile : "(null)");

    return 0;
}