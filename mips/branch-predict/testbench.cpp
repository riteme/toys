#include <vector>
#include <numeric>
#include <algorithm>

#include "testbench.h"
#include "device.h"

// #define DISABLE_REFERENCE

/**
 * SETUP
 */

static auto ref = new ReferencePredictor;

#ifdef DISABLE_REFERENCE
static auto dev = new Device;
#else
static auto dev = new Device(/* ref */);
#endif

SETUP_TESTLIST
PRETEST_HOOK = [] {
    dev->reset();
};
POSTTEST_HOOK = [] {
    dev->close_report();
};

#define IF(expr) if (dev->lookup((expr), __LINE__, randu(1, 100)))
#define FOR(initial, condition, final) \
    for (initial; dev->lookup((condition), __LINE__, -10); final)

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

void _qsort(int *a, int l, int r) {
    if (r <= l)
        return;

    int pivot = a[l];
    int p = l;
    // for (int i = l; i <= r; i++) {
    FOR (int i = l, i <= r, i++) {
        // if (a[i] < pivot)
        IF (a[i] < pivot)
            std::swap(a[i], a[p++]);
    }

    int q = p;
    // for (int i = p; i <= r; i++) {
    FOR (int i = p, i <= r, i++) {
        // if (a[i] <= pivot)
        IF (a[i] <= pivot)
            std::swap(a[i], a[q++]);
    }

    _qsort(a, l, p - 1);
    _qsort(a, q, r);
}

WITH REPORT_TO("result/qsort.out") STATISTICS {
    int n = 500;

    int a[n];
    for (int i = 0; i < n; i++) {
        a[i] = randu();
    }

    _qsort(a, 0, n - 1);
    assert(std::is_sorted(a, a + n));
} AS("qsort");

WITH REPORT_TO("result/calc.out") STATISTICS {
    int n = 1000;

    int ans = randu();
    // for (int i = 0; i < n; i++) {
    FOR (int i = 0, i < n, i++) {
        // if (i % 2 == 0) {
        IF (i % 2 == 0) {
            ans ^= ans << 7;
        }
        // if (i % 3 == 1) {
        IF (i % 3 == 1) {
            ans ^= ans << 17;
        }
        // if ((i & 3) >= 1) {
        IF ((i & 3) >= 1) {
            ans ^= ans >> 11;
        }
        // if ((i % 3) ^ (i % 2)) {
        IF ((i % 3) ^ (i % 2)) {
            ans ^= ans >> 20;
        }
        // if ((i & 3) < 2) {
        IF ((i & 3) < 2) {
            ans ^= ans << 3;
        }
        // if (i % 9 > 3) {
        IF (i % 9 > 3) {
            ans ^= ans << 12;
        }
        // if ((i & 3) < 3) {
        IF ((i & 3) < 3) {
            ans ^= ans >> 6;
        }
    }

    printf("ans = %d\n", ans);
} AS("calc");

WITH REPORT_TO("result/collatz.out") STATISTICS {
    int n = 500, cnt = 0;
    // for (int i = 1; i <= n; i++) {
    FOR (int i = 1, i <= n, i++) {
        int x = i;
        while (true) {
            cnt++;

            // if (x % 2 == 1) {
            IF (x % 2 == 1) {
                x = 3 * x;
            }
            // if (x % 2 != 0 && x % 3 == 0) {
            IF (x % 2 != 0 && x % 3 == 0) {
                x = x + 1;
            }
            // if (x % 2 == 0) {
            IF (x % 2 == 0) {
                x = x / 2;
            }
            // if (x == 1) {
            IF (x == 1) {
                break;
            }
        }
    }

    printf("cnt = %d\n", cnt);
} AS("collatz");