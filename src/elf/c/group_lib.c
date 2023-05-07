#include <stdio.h>
#include "group_lib.h"

const char *default_level = LEVEL_INFO;

void log2(const char *level, const char *message) {
    printf("%s: %s\n", level, message);
}

void log(const char *message) {
    log2(default_level, message);
}
