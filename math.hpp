#ifndef CHEUKYIN_RAYTRACER_MATH_H
#define CHEUKYIN_RAYTRACER_MATH_H

#include <math.h>

namespace RayTracer{
    namespace Math
    {
        const double EPSILON = 0.000001;
        const double PI = 3.141592653;

        inline bool is_double_eq(double v1, double v2)
        { return fabs(v1 - v2) < EPSILON; }
        inline bool is_double_zero(double v)
        { return fabs(v - 0) < EPSILON; }
    } // namespace Math
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_MATH_H