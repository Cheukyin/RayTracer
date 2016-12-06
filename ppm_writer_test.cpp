#include "unittest.hpp"
#include "ppm_writer.hpp"

using namespace RayTracer::ImageWriter;

TEST_CASE(TestPPMWriter)
{
    const RGB r{ 255, 0, 0 };
    const RGB g{ 0, 255, 0 };
    const RGB b{ 0, 0, 255 };

    PPMWriter<3, 3, 255> ppm;

    for(int i = 0; i < 3; i++)
    {
        ppm[i][0] = r;
        ppm[i][1] = g;
        ppm[i][2] = b;
    }

    ppm.write("rgb3by3.ppm");
}