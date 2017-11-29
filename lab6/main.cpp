//Emilio Diaz

#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>

#define W 3
#define H 4
#define NUM_THREADS 10

using namespace cv;
using namespace std;

void *create_image(void* numero){
    cout<<"thread calledd"<<endl;
        int res;
    for (int i = 0; i<H; i++){
        for(int j = 0; j <W; j++){
                        res=i*j;
                }
        }
    pthread_exit((void*) res);

}

void pixelate(Mat& src, Mat& dst, int pixel_size = 1) {
    try {
        // crear cv::Mat de salida, de igual tamano que la imagen src
        dst.create(src.rows, src.cols, src.type());

        Rect rect;

        for (int r = 0; r < src.rows; r += H*W){
            for (int c = 0; c < src.cols; c += H*W){
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
    pixelate(*((Mat*)data), dst2, W*H);

    imshow("Pixelate Effect", dst2);
	imwrite( "newImage.jpg", dst2 );

}

int main(int argc, char** argv){
    Mat image = imread("ramphastosSulphuratus.jpeg", CV_LOAD_IMAGE_COLOR);
    pthread_t threads[NUM_THREADS];
    int t = 0;

//	for (t = 0; t <= NUM_THREADS; t++){
  //      pthread_create(&threads[t], NULL, create_image, (void *) t);
   // }
    ontrack(H*W, &image);


    return 0;
}

