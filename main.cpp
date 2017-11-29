//Universidad del Valle de Guatemala
//Serial implementation of mandelbrot set
//Christian Medina
//Emilio Diaz

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
//se importa el queue y el semaphore
#include <queue>
#include <semaphore.h>



using namespace std;
using namespace cv;

#define size_y 1000
#define size_x size_y*1.125
//cantidad de threads optimo
#define NUM_THREADS 32

//compute numer of iterations to diverge
int mandelbrotIterations(const complex<float> &z0, const int max){
    complex<float> z = z0;
    for (int t = 0; t < max; t++){
        if( (z.real()*z.real() + z.imag()*z.imag() ) > 4.0f){
           return t;
        }
        z = z*z + z0;
    }

    return max;
}

//assign grayscale value 
int mandelbrotSet(const complex<float> &z0, const int maxIter=500) {
    //does it diverge?
    int iterations = mandelbrotIterations(z0, maxIter);
    
    //avoid division by zero
    if(maxIter - iterations == 0){
        return 0;
    }

    //rescale value to 8 bits (CV_U8)
    return cvRound(sqrt(iterations / (float) maxIter) * 255);
}

//funcion del thread
void threadFn() {
    //si no se ha terminado el stack
    if(!myqueue.empty()){
        var gpop= myqueue.pop();

        //operaciones de imaginarios
        float x0 = qpop[0] / scaleX + x1;
        float y0 = qpop[1] / scaleY + y1;

        complex<float> z0(x0, y0);
        uchar value = (uchar) mandelbrotSet(z0);

        //se escribe a imagen data del fractal
        sem_wait(&mySemaphore);
        img.ptr<uchar>(qpop[0])[qpop[1]] = value;
        sem_post(&mySemaphore);
    }
    return 0;

}

//globales

//dimensiones del fractales
float x1 = -2.1f;
float x2 =  0.6f;
float y1 = -1.2f;
float y2 =  1.2f;

//escalas del fractal
float scaleX = mandelbrotImg.cols / (x2 - x1);
float scaleY = mandelbrotImg.rows / (y2 - y1);
//queue de datos
std::queue<int> myqueue;
//semaphore
sem_t mySemaphore;

//!globales

int main(int argc, char *argv[]){

    //prepare variables
    Mat mandelbrotImg(size_y, size_x, CV_8U);
    //queue donde se almacenan los valores

    //se llena el queue
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            int temp[2]= {i,j};
            myqueue.push (temp);
        }
    }

    //iniciar el semaphore
    sem_init(&mySemaphore,0,1);

    //create threads
    for(t=0; t<NUM_THREADS; t++){
        cout<<"Creando Thread No."<< t<< endl;
        rc = pthread_create(&threads[t], NULL, threadFn, (void *)t);

        if (rc){
            cout<<"ERROR: return code from pthread_create() <<"<< rc<< endl;
            return(-1);
        }
    }

    //compute the fractal
    double t2 = (double) getTickCount();


    //mandelbrot_serial(mandelbrotImg, x1, y1, scaleX, scaleY);
    t2 = ((double) getTickCount() - t2) / getTickFrequency();

    //print execution time
    cout << "Execution time: " << t2 << " s" << endl;

    //store computed fractal image
    imwrite("Mandelbrot_serial.png", mandelbrotImg);

    return(0);
}
