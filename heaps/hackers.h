#pragma once

#include "framework.h"

namespace hack {
    void initiailize(i64 _seed);
    void uniform(ShortestPath &instance, int s);
    void max_decrease(ShortestPath &instance, int s);
}