#include "unittest.hpp"
#include "ppm_writer.hpp"

using namespace RayTracer::ImageWriter;

TEST_CASE(TestPPMWriter)
{
    const RGB r{ 255, 0, 0 };
    const RGB g{ 0, 255, 0 };
    const RGB b{ 0, 0, 255 };

    const int Width = 1000;
    const int Height = 900;
    const int MaxVal = 255;
    PPMWriter ppm(Height, Width, MaxVal);

    int step = Width / 3;
    for(int h = 0; h < Height; h++)
    {
        for(int w = 0; w < Width; w++)
        {
            int part = w / step;
            if(part == 0)      ppm[h][w] = r;
            else if(part == 1) ppm[h][w] = g;
            else               ppm[h][w] = b;
        }
    }

    ppm.write("rgb3by3.ppm");
}