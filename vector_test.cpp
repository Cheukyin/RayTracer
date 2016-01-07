#include "unittest.hpp"
#include "vector.hpp"

typedef RayTracer::Math::Vector3d<double> Vec3d;

TEST_CASE(TestVectorEquality)
{
    Vec3d v0(1, 2.1, 0);
    Vec3d v1(1, 2.1, 0);
    Vec3d v2(1, 2, 0.3);

    EXPECT_EQ(v0, v1);
    EXPECT_NEQ(v0, v2);
}

TEST_CASE(TestVectorIndex)
{
    Vec3d v(1, 2.1, 0.2);

    EXPECT_DOUBLE_EQ(v[0], 1);
    EXPECT_DOUBLE_EQ(v[1], 2.1);
    EXPECT_DOUBLE_EQ(v[2], 0.2);
}

TEST_CASE(TestVectorScale)
{
    Vec3d v(1.2, 2.1, 0.2);
    Vec3d r(3.72, 6.51, 0.62);

    EXPECT_EQ(3.1*v, r);
    EXPECT_EQ(v*3.1, r);
    EXPECT_EQ(v.scale(3.1), r);
}

TEST_CASE(TestVectorAdd)
{
    Vec3d v0(1, 3, 0.3);
    Vec3d v1(2.3, 2.1, 0.3);
    Vec3d r(3.3, 5.1, 0.6);

    EXPECT_EQ(v0+v1, r);
    EXPECT_EQ(v0.add(v1), r);
    EXPECT_EQ(v0, r);
}

TEST_CASE(TestVectorSub)
{
    Vec3d v0(1, 3, 0.3);
    Vec3d v1(2.3, 2.1, 0.3);
    Vec3d r(-1.3, 0.9, 0);

    EXPECT_EQ(v0-v1, r);
    EXPECT_EQ(v0.sub(v1), r);
    EXPECT_EQ(v0, r);
}

TEST_CASE(TestVectorDot)
{
    Vec3d v0(1, 3, 0.3);
    Vec3d v1(2.3, 2.1, 0.3);

    EXPECT_DOUBLE_EQ(v0.dot(v1), 8.69);
}

TEST_CASE(TestVectorIsNull)
{
    Vec3d v0(1, 3, 0);
    Vec3d v1(0, 0, 0);

    EXPECT_FALSE( is_null(v0) );
    EXPECT_TRUE( is_null(v1) );
}

TEST_CASE(TestVectorIsPerpendicular)
{
    Vec3d v0(1, 3, 0);
    Vec3d v1(0, 0, 0.3);
    Vec3d v2(1, 3, 3);

    EXPECT_TRUE( is_perpendicular(v0, v1) );
    EXPECT_FALSE( is_perpendicular(v0, v2) );
}

TEST_CASE(TestVectorCross)
{
    Vec3d v0(1, 3, 4);
    Vec3d v1(2.3, 2.1, 0.3);

    EXPECT_TRUE( is_perpendicular(v0*v1, v0) );
    EXPECT_TRUE( is_perpendicular(v0*v1, v1) );

    Vec3d v2(1, 2, 3);
    Vec3d v3(4, 5, 6);
    Vec3d r(-3, 6, -3);

    EXPECT_EQ(v2*v3, r);
    EXPECT_EQ(v2.cross(v3), r);
    EXPECT_EQ(v2, r);
}

TEST_CASE(TestVectorLength)
{
    Vec3d v(1, 2, 3);
    EXPECT_DOUBLE_EQ(v.sqrlength(), 14);
    EXPECT_DOUBLE_EQ(v.length(), 3.741657);
}

TEST_CASE(TestVectorNormalize)
{
    Vec3d v(1.23, 2.4, 3.22);
    Vec3d r = normalize(v);

    EXPECT_DOUBLE_EQ(r.length(), 1);
    EXPECT_DOUBLE_EQ(r.dot(v) / v.length(), 1);
    EXPECT_EQ(v.normalize(), r);
}