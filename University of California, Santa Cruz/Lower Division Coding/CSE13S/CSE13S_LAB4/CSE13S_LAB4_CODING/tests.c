#include "mathlib.h"
#include "operators.h"
#include "stack.h"

#include <math.h>
#include <stdio.h>

int main(void) {
    double x = 1.0, y = 2.0, z = 3.0;
    stack_push(x);
    stack_peek(&x);
    stack_push(y);
    stack_peek(&y);
    stack_push(z);
    stack_peek(&z);
    if ((x != 1.0) || (y != 2.0) || (z != 3.0))
        return 1;
    stack_pop(&x);
    stack_pop(&y);
    stack_pop(&z);
    if ((x != 3.0) || (y != 2.0) || (z != 3.0))
        return 1;
    stack_clear();
    if (stack_size != 0)
        return 1;
    for (double i = -2.0 * M_PI; (!(i > 2.0 * M_PI)); i += M_PI / 12.0)
        if ((Sin(i) != sin(i)) || (Cos(i) != cos(i)) || (Tan(i) != tan(i)) || (Sqrt(i) != sqrt(i))
            || (Abs(i) != fabs(i)) || (operator_add(i, i) != i + i) || (operator_sub(i, i) != 0.0)
            || (operator_mul(i, i) != i * i) || (operator_div(i, i) != 1.0))
            return 1;
    return 0;
}
