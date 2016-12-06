#ifndef CHEUKYIN_RAYTRACER_PPM_WRITER_H
#define CHEUKYIN_RAYTRACER_PPM_WRITER_H

#include <string.h>
#include <string>
#include <fstream>

namespace RayTracer{
    namespace ImageWriter
    {
        struct RGB { int r; int g; int b; };

        std::ostream& operator<<(std::ostream& os, RGB& pixel)
        { 
            os << pixel.r << " " << pixel.g << " " << pixel.b;
            return os;
        }

        template<int Width, int Height, int MaxVal = 255>
        class PPMWriter
        {
        public:
            PPMWriter() 
            { memset(matrix, 0, sizeof(matrix)); }

            RGB* operator[](int h)
            { return matrix[h]; }

            void write(const std::string& filepath)
            {
                std::ofstream outf;
                outf.open(filepath);

                outf << "P3" << std::endl;
                outf << Width << " " << Height << std::endl;
                outf << MaxVal << std::endl;

                for(int i = 0; i < Height; i++)
                {
                    for(int j = 0; j < Width; j++)
                        outf << matrix[i][j] << " ";
                    outf << std::endl;
                }

                outf.close();
            }

        private:
            RGB matrix[Height][Width];

        }; // class PPMWriter

    } // namespace ImageWriter
} // namespace RayTracer


#endif // CHEUKYIN_RAYTRACER_PPM_WRITER_H