#include <math.h>
#include <stdio.h>
#define EPSILON 1e-14

double Abs(double x) {
    if (x < 0.0)
        return -x;
    return x;
}

double Sqrt(double x) {
    if (x < 0)
        return nan("nan");
    double old = 0.0, new = 1.0;
    while (Abs(old - new) > EPSILON) {
        old = new;
        new = 0.5 * (old + (x / old));
    }
    return new;
}

double Sin(double x) {
    x = fmod(x, 2 * M_PI);
    double sinval = 0.0, term = x, n = 2.0;
    while (fabs(term) > EPSILON) {
        sinval += term;
        term *= (-1.0 * x * x) / (n * (n + 1.0));
        n += 2.0;
    }
    return sinval;
}

double Cos(double x) {
    x = fmod(x, 2 * M_PI);
    double cosval = 0.0, term = 1.0, n = 1.0;
    while (fabs(term) > EPSILON) {
        cosval += term;
        term *= (-1.0 * x * x) / (n * (n + 1.0));
        n += 2.0;
    }
    return cosval;
}

double Tan(double x) {
    return Sin(x) / Cos(x);
}
