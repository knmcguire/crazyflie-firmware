#include <stdio.h>
#include <stdlib.h>

void assertFail(char *exp, char *file, int line) {
    printf("ASSERTION FAILED: %s in file %s, line %d\n", exp, file, line);
    exit(1);
}
