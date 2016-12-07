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

        class PPMWriter
        {
        public:
            PPMWriter(int height, int width, int maxVal = 255)
                : height(height), width(width), maxVal(maxVal)
            { 
                matrix = new RGB*[height];
                for (int h = 0; h < height; h++)
                {
                    matrix[h] = new RGB[width];
                    memset(matrix[h], 0, width * sizeof(matrix[h][0]));
                }
            }

            RGB* operator[](int h)
            { return matrix[h]; }

            void write(const std::string& filepath)
            {
                std::ofstream outf;
                outf.open(filepath);

                outf << "P3" << std::endl;
                outf << width << " " << height << std::endl;
                outf << maxVal << std::endl;

                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < width; j++)
                        outf << matrix[i][j] << " ";
                    outf << std::endl;
                }

                outf.close();
            }

            ~PPMWriter()
            {
                for(int h = 0; h < height; h++)
                    delete[] matrix[h];
                delete[] matrix;
            }

        private:
            int height;
            int width;
            int maxVal;
            RGB **matrix;

        }; // class PPMWriter

    } // namespace ImageWriter
} // namespace RayTracer


#endif // CHEUKYIN_RAYTRACER_PPM_WRITER_H