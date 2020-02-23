initially there is a 0/1 sequece of length $n$. Each time you can query on some set of indices to get the number of 1 in those indices.

**Goal:** minimize the number of queries to obtain enough information to rebuild the sequence from sratch.

**Progress:** ratios of the number of queries w.r.t. $n$.

* `4ary.cpp`: 0.625
* `main.cpp`: 0.6 (actually ~0.61)
* `async.cpp`: 0.6

**Conjecture on lower bound:** 0.5