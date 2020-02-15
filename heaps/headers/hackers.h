#pragma once

#include "framework.h"

#include <string>
#include <functional>

#define AVAILABLE_HACKERS "uniform span stack"

namespace hack {
    using Graph = ::ShortestPath;
    using Hacker = std::function<void(Graph&, int)>;

    void initiailize(i64 _seed);
    auto get_hacker(const std::string &name) -> Hacker;

    void uniform(Graph &G, int s);
    void span(Graph &G, int s);
    void stack(Graph &G, int s);
}