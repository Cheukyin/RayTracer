#ifndef CHEUKYIN_RAYTRACER_VECTOR_H
#define CHEUKYIN_RAYTRACER_VECTOR_H

#include <ostream>
#include <math.h>
#include "math.hpp"

namespace RayTracer{
    namespace Math
    {
        template<class T>
        class Vector3d{
        public:
            Vector3d()
            {
                v[0] = 0;
                v[1] = 0;
                v[2] = 0;
            }
            Vector3d(T x, T y, T z)
            {
                v[0] = x;
                v[1] = y;
                v[2] = z;
            }


            T& operator[](int n) // 0, 1, 2 => x, y, z
            { return v[n]; }
            T operator[](int n) const // 0, 1, 2 => x, y, z
            { return v[n]; }


            Vector3d<T>& scale(T scalar)
            {
                v[0] *= scalar;
                v[1] *= scalar;
                v[2] *= scalar;
                return *this;
            }


            Vector3d<T>& add(const Vector3d<T>& vec)
            {
                v[0] += vec[0];
                v[1] += vec[1];
                v[2] += vec[2];
                return *this;
            }


            Vector3d<T>& sub(const Vector3d<T>& vec)
            {
                v[0] -= vec[0];
                v[1] -= vec[1];
                v[2] -= vec[2];
                return *this;
            }


            T dot(const Vector3d<T>& vec) const
            { return v[0]*vec[0] + v[1]*vec[1] + v[2]*vec[2]; }


            Vector3d<T>& cross(const Vector3d<T>& vec)
            {
                Vector3d<T> tmp(*this);
                v[0] = tmp[1]*vec[2] - tmp[2]*vec[1];
                v[1] = tmp[2]*vec[0] - tmp[0]*vec[2];
                v[2] = tmp[0]*vec[1] - tmp[1]*vec[0];
                return *this;
            }


            T sqrlength() const
            { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2]; }
            double length() const
            { return sqrt( sqrlength() );  }


            Vector3d<T>& normalize()
            {
                double dist = length();
                if(dist)
                {
                    v[0] /= dist;
                    v[1] /= dist;
                    v[2] /= dist;
                }
                return *this;
            }


        private:
            T v[3]; // x, y , z
        };


        template<class T>
        Vector3d<T> operator*(T scalar, const Vector3d<T>& vec)
        {
            Vector3d<T> new_v(vec);
            new_v.scale(scalar);
            return new_v;
        }
        template<class T>
        Vector3d<T> operator*(const Vector3d<T>& vec, T scalar)
        { return scalar * vec; }


        template<class T>
        bool is_null(const Vector3d<T>& vec)
        {
            return is_double_zero(vec[0])
                && is_double_zero(vec[1])
                && is_double_zero(vec[2]);
        }


        template<class T>
        bool is_perpendicular(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        { return is_double_zero( vec1.dot(vec2) ); }


        template<class T>
        Vector3d<T> operator+(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        {
            Vector3d<T> new_v(vec1);
            new_v.add(vec2);
            return new_v;
        }


        template<class T>
        Vector3d<T> operator-(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        {
            Vector3d<T> new_v(vec1);
            new_v.sub(vec2);
            return new_v;
        }


        template<class T>
        Vector3d<T> operator*(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        {
            Vector3d<T> new_v;
            new_v[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
            new_v[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
            new_v[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
            return new_v;
        }


        template<class T>
        bool operator==(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        {
            return is_double_eq(vec1[0], vec2[0]) \
                && is_double_eq(vec1[1], vec2[1]) \
                && is_double_eq(vec1[2], vec2[2]);
        }
        template<class T>
        bool operator!=(const Vector3d<T>& vec1, const Vector3d<T>& vec2)
        { return !(vec1 == vec2); }


        template<class T>
        Vector3d<T> normalize(const Vector3d<T>& vec)
        {
            Vector3d<T> new_v(vec);
            double dist = new_v.length();
            if(dist)
            {
                new_v[0] /= dist;
                new_v[1] /= dist;
                new_v[2] /= dist;
            }
            return new_v;
        }


        template<class T>
        std::ostream& operator<<(std::ostream &os, const Vector3d<T>& vec)
        { return os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")"; }

    } // namespace Math
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_VECTOR_H