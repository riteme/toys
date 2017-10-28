# Resistance Calculator
Calculate the resistance of simple networks made up of pure resistances.

## Files

*   `resist.py`: Simple utility for resistance calculations.
*   `reisistances-of-all-pairs.cpp`: For algorithm competitions' use (ACM, OI, etc.), which implemented an algorithm that can calculate the resistances of all pairs "`(source, sink)`" in $\Theta(m^3)$ time complexity, where $m$ represents the number of edges in the resistance network. Since the program doesn't handle the case of unconnected graphs, it guranteed that $m \geqslant n - 1$ ($n$ is the number of nodes in the network).
*   `test.*`: Test results produced by `resist.py`.

## Requirements
* Python3
* SymPy
* C++ compiler with at least C++11 support.

## TODO List

- [x] Graphviz output