#pragma once

#include "framework.h"

#include <string>
#include <functional>

#define AVAILABLE_HACKERS "uniform span stack"

namespace hack {
    typedef ShortestPath Graph;
    typedef std::function<void(Graph&, int)> Hacker;

    void initiailize(i64 _seed);
    auto get_hacker(const std::string &name) -> Hacker;

    void uniform(Graph &G, int s);
    void span(Graph &G, int s);
    void stack(Graph &G, int s);
}