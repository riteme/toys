#include "framework.h"
#include "hackers.h"

#include <unistd.h>

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstdarg>

#include <vector>
#include <chrono>
#include <algorithm>

#define CLR "\r\033[2K"

constexpr int STATUS_LINE_MAXLEN = 256;
constexpr i64 DEFAULT_SEED = 0x19260817deedbeef;
constexpr i64 DEFAULT_EMAX = 10'0000'0000;

static i64 seed = DEFAULT_SEED;
static i64 EMAX = DEFAULT_EMAX;
static int repeat = 1;
static i64 max_task_bound = -1;
static bool enable_hack = false;
static bool quiet = false;
static char *filename = NULL;
static FILE *file_handle = NULL;

void parse_opt(int argc, char *argv[]) {
    char c;

    const char *opts = ":huqs:r:e:m:o:";
    while ((c = getopt(argc, argv, opts)) != -1)
    switch (c) {
        case 'h': printf(
            "Usage: %s [%s]\n"
            "   -h  print this message\n"
            "   -u  enable \"max_decrease\" hacker\n"
            "   -q  output less information\n"
            "   -s  specify the seed for hacker's random genenrator\n"
            "   -r  number of runs for one task\n"
            "   -e  expected maximum number of edges to be processed\n"
            "   -m  maximum number of tasks\n"
            "   -o  save stdout to file\n",
            argv[0], opts + 1
        ); exit(0);
        case 'u': enable_hack = true; break;
        case 'q': quiet = true; break;
        case 's': seed = atoll(optarg); break;
        case 'r': repeat = atoi(optarg); break;
        case 'e': EMAX = atoll(optarg); break;
        case 'm': max_task_bound = atoll(optarg); break;
        case 'o': filename = optarg; break;
        case ':': fprintf(stderr, "Option \"-%c\" needs argument.\n", optopt); exit(-1);
        case '?': fprintf(stderr, "Unknown option \"-%c\".\n", optopt); exit(-1);
        default: abort();
    }

    assert(1 <= repeat);
    assert(0 <= EMAX);

    if (filename) {
        file_handle = fopen(filename, "w");
        if (!file_handle) {
            fprintf(stderr, "Failed to open file \"%s\".", filename);
            exit(-1);
        }
    }
}

auto initialize() -> ShortestPath* {
    int n, m;
    scanf("%d%d", &n, &m);

    auto instance = new ShortestPath(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        instance->link(u, v);
    }

    return instance;
}

void clear_line() {
    fprintf(stderr, CLR);
    fflush(stderr);
}

void status_line(int progress, i64 _estimate, const char *_status, ...) {
    va_list args;
    va_start(args, _status);
    char status[STATUS_LINE_MAXLEN];
    vsnprintf(status, STATUS_LINE_MAXLEN, _status, args);

    char estimate[STATUS_LINE_MAXLEN] = "(estimating...)";
    if (_estimate > 0) {
        i64 hours, mins, secs;
        hours = _estimate / 3600;
        mins = _estimate % 3600 / 60;
        secs = _estimate % 60;
        snprintf(estimate, STATUS_LINE_MAXLEN,
            "(%lld:%02lld:%02lld remaining)", hours, mins, secs);
    } else if (_estimate == 0)
        estimate[0] = '\0';

    if (isatty(STDOUT_FILENO))
        fprintf(stderr,
            CLR "[%3d%%] %-16s %-11s %-8s %-8s %s\t\"%s\" %s",
            progress, "hash", "time (ms)", "aux", "count", "ratio",
            status, estimate
        );
    else
        fprintf(stderr, CLR "[%d%%] %s %s", progress, status, estimate);
    fflush(stderr);
}

void output(bool front, const char *fmt, ...) {
    va_list args;

    if (front) {
        va_start(args, fmt);
        vprintf(fmt, args);
    }

    if (file_handle) {
        va_start(args, fmt);
        vfprintf(file_handle, fmt, args);
    }
}

void prepare(ShortestPath *instance, int s) {
    if (enable_hack)
        hack::max_decrease(*instance, s);
    else
        hack::uniform(*instance, s);
}

auto average(const std::vector<double> &li) -> double {
    double sum = 0;
    auto maxp = repeat > 1 ?
        std::max_element(li.begin(), li.end()) : li.end();

    for (auto it = li.begin(); it != li.end(); it++)
        if (it != maxp) sum += *it;

    return sum / std::max(1, repeat - 1);
}

int main(int argc, char *argv[]) {
    using namespace std::chrono;

    parse_opt(argc, argv);
    auto instance = initialize();
    hack::initiailize(seed);

    i64 max_task = std::min(
        static_cast<i64>(instance->n),
        (EMAX + instance->m - 1) / instance->m
    );
    if (max_task_bound >= 0 && max_task > max_task_bound)
        max_task = max_task_bound;

    if (!quiet)
        fprintf(stderr,
            "n = %d; m = %d; max_task = %lld; seed = 0x%llx\n",
            instance->n, instance->m, max_task, seed
        );
    output(!isatty(STDOUT_FILENO),
        "n = %d; m = %d; seed = 0x%llx; hack = %d\n",
        instance->n, instance->m, seed, enable_hack
    );

    i64 estimate = -1;
    auto t_start = high_resolution_clock::now();
    for (i64 i = 1; i <= max_task; i++) {
        ShortestPath::Result result;

        status_line(100 * (i - 1) / max_task, estimate, "Preparing for #%d", i);
        prepare(instance, i);

        std::vector<double> records;
        records.reserve(repeat);

        for (int j = 0; j < repeat; j++) {
            status_line(
                100 * ((i - 1) * repeat + j) / repeat / max_task,
                estimate, "Running #%d.%d", i, j + 1
            );

            result = instance->solve(i);
            records.push_back(result.time);
        }

        result.time = average(records);

        clear_line();
        output(true,
            "  %-4lld %.16llx %-11.6f %-8d %-8d %.2fx\t",
            i, result.hash, result.time, result.aux, result.count,
            static_cast<double>(result.count) / instance->n
        );
        for (auto tm : records)
            output(true, "%.1f ", tm);
        output(true, "\n");

        auto t_end = high_resolution_clock::now();
        auto ms = duration_cast<milliseconds>(t_end - t_start).count();
        estimate = std::max((max_task - i) * ms / i / 1000, 1LL);
    }

    status_line(100, 0, "Finished.");
    fputs("\n", stderr);

    delete instance;
    return 0;
}