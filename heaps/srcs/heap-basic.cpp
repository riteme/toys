#include "framework.h"

namespace HeapInterface {

ShortestPath *instance;
int n;
int _aux;
i64 *w;

void initialize(ShortestPath *_instance) {
    instance = _instance;
    n = instance->n;
    w = instance->dist;
}

}