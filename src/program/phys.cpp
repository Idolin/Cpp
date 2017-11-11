#include "phys.h"

double *
fdm(Expression fa, Expression fb, Expression fc, Expression fd, double lb, double rb, double lvalue, double rvalue,
    unsigned points)
{
    double step = (rb - lb) / ++points;
    Expression st = new Const(step);
    Expression prev = new Sub(new Mult(2, fa), new Mult(fb, st));
    Expression curr = new Add(new Mult(-4, fa), new Mult(new Mult(2, fc), new Pwr(st, 2)));
    Expression next = new Add(new Mult(2, fa), new Mult(fb, st));
    Expression e = new Mult(new Mult(2, fd), new Pwr(st, 2));
    double *a = new double[points + 1], *b = new double[points];
    a[0] = 0;
    b[0] = lvalue;
    for(unsigned i = 1; i < points; i++)
    {
        double x = lb + i * step;
        a[i] = -next(x) / (prev(x) * a[i - 1] + curr(x));
        b[i] = (e(x) - prev(x) * b[i - 1]) / (prev(x) * a[i - 1] + curr(x));
    }
    a[points] = rvalue;
    while(points-- > 0)
        a[points] = a[points] * a[points + 1] + b[points];
    delete[] b;
    return a;
}
