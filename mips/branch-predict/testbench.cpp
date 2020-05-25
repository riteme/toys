#include <vector>
#include <numeric>
#include <algorithm>

#include "testbench.h"
#include "device.h"

/**
 * SETUP
 */

static auto ref = new ReferencePredictor;
static auto dev = new Device(ref);

SETUP_TESTLIST
PRETEST_HOOK = [] {
    dev->reset();
};
POSTTEST_HOOK = [] {
    // do nothing
};

#define IF(expr) if (dev->lookup((expr), __LINE__, randu(1, 100)))

/**
 * TESTBENCH
 */

WITH {
    dev->nop();
} AS("nop");

WITH {
    IF (1 == 1) {}
} AS("single branch");

WITH {
    IF (1 == 1) {}
    IF (1 == 1) {}
} AS("double branches");

WITH {
    IF (1 == 1) {

    }
    IF (1 == 1) {

    }
} AS("double branches 2");

WITH {
    for (int i = 0; i < 10; i++) {
        IF (1 == 1) {}
    }
} AS("repeat branch");

WITH {
    for (int i = 0; i < 100; i++) {
        IF (1 == 1) {

        }
        IF (1 == 1) {

        }
    }
} AS("repeat double branches");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        int a = randu(0, 100);
        IF (a < 90) {}
    }
} AS("bias short");

WITH STATISTICS {
    for (int i = 0; i < 5000; i++) {
        int a = randu(0, 100);
        IF (a < 90) {}
    }
} AS("bias long");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        int a = randu(0, 100), b;
        // printf("a = %d\n", a);

        IF (a < 90) {
            b = 100;
        }
        IF (a >= 90) {
            b = 0;
        }
        IF (b == 100) {}
    }
} AS("bias & global correlation");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        IF (i & 1) {}
    }
} AS("repeat pattern");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        IF (i % 2 == 0) {}
        IF (i % 3 == 0) {}
        IF (i % 4 == 0) {}
    }
} AS("local repeat pattern");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        IF (i % 2 == 0) {
            // On a dark desert highway,
            // cool wind in my hair.
        }
        IF (i % 3 == 0) {
            // Warm smell of colitas,
            // rising up through the air.
        }
        IF (i % 4 == 0) {
            // Up ahead in the distance,
            // I saw a shimmering light.
        }
    }
} AS("local repeat pattern 2");

WITH STATISTICS {
    for (int i = 0; i < 500; i++) {
        IF (i % 2 == 0) {
            // On a dark desert highway,
            // cool wind in my hair.
            // we saying~~~~~
            // we song~~~~~~~
            // we saying~~~~~
            // we song~~~~~~~
        }
        IF (i % 3 == 0) {
            // Warm smell of colitas,
            // rising up through the air.
            // we saying~~~~~
            // we song~~~~~~~
            // we saying~~~~~
            // we song~~~~~~~
        }
        IF (i % 4 == 0) {
            // Up ahead in the distance,
            // I saw a shimmering light.
            // we saying~~~~~
            // we song~~~~~~~
            // we saying~~~~~
            // we song~~~~~~~
        }
    }
} AS("local repeat pattern 3");

#define FOR(initial, condition, final) \
    for (initial; dev->lookup((condition), __LINE__, -10); final)

WITH STATISTICS {
    for (int i = 0; i < 100; i++) {
        FOR (int j = 0, j < 5, j++) {
            // do nothing
        }
    }
} AS("for loop");

WITH STATISTICS {
    for (int i = 0; i < 100; i++) {
        FOR (int j = 0, j < 5, j++)
        FOR (int k = 0, k < 5, k++)
        FOR (int l = 0, l < 5, l++) {}
    }
} AS("nested for loops");

/**
 * TODO:
 * * quick sort
 * * floyd
 */