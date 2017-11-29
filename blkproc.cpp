#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void pixelate(Mat& src, Mat& dst, int pixel_size = 1) {
    try {
        // crear cv::Mat de salida, de igual tamano que la imagen src
        dst.create(src.rows, src.cols, src.type());

        Rect rect;

        for (int r = 0; r < src.rows; r += pixel_size)
        {
            for (int c = 0; c < src.cols; c += pixel_size)
            {
                rect.x = c;
                rect.y = r;
                rect.width = c + pixel_size < src.cols ? pixel_size : src.cols - c;
                rect.height = r + pixel_size < src.rows ? pixel_size : src.rows - r;

                // obtener el color promedio del area indicada
                Scalar color = mean(Mat(src, rect));

                // pintar el area indicada con el color obtenido
                rectangle(dst, rect, color, CV_FILLED);
            }
        }
    }
    catch (cv::Exception &ex) {
        cout << ex.what() << endl;
    }
}

void ontrack(int value, void* data) {
    if (value <= 0) return;

    Mat dst1, dst2;

    // pixelar la imagen completa
    pixelate(*((Mat*)data), dst2, value);

    imshow("Pixelate Effect", dst2);
}

int main(int argc, char** argv)
{
    Mat image = imread("ramphastosSulphuratus.jpeg", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) {
        printf("No image data \n");
        return -1;
    }

    int pixel_size = 8;

    namedWindow("Pixelate Effect", WINDOW_AUTOSIZE);
    createTrackbar("Pixel Size", "Pixelate Effect", &pixel_size, 50, ontrack, &image);
    ontrack(pixel_size, &image);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
