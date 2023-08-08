#include <stdio.h>
#include "log_lib.h"

const char *default_level = LEVEL_INFO;

static void log_mini_static(const char *level, const char *message) {
    printf("%s: %s\n", level, message);
}

void log_mini(const char *message) {
    log_mini_static(default_level, message);
}
