# Root Finding Programs

## For Polynomials

It works fine just on low-degree polynomials (usually less than 7) with small coefficients.

It has been f**ked by this data:

```
9
1 -71.4623784930137 2259.45469795742 -41474.0815040708 486948.744923884 -3791351.53329433 19569143.4305392 -64545811.1539629 123398664.785574 -104135718.823395
```

which has 9 rational solution:

```
4.8940624439311184
6.603865749553303
6.610018024496641
8.161534869887534
8.304589905429467
8.438380868314713
9.247927636307958
9.457418931558202
9.744580063534759
```

**NOTICE**: Input format:

```
[degree of the polynomial]
a_n a_{n - 1} ... a_0  // Represents a_n * x^n + a_{n - 1} * x^{n - 1} + ... + a_0 = 0
```

## Requirements

*   C++ compiler with at least C++11 support (GCC, clang, etc.)