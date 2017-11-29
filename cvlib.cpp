//Universidad del Valle de Guatemala
//Christian Medina Armas


#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "cvlib.h"

using namespace std;
using namespace cv;

//fucntion implementation
//-----------------------------------------------------------
int imgShowLab(const string& winName, Mat& src,int size, int pos_x, int pos_y){

//get mage size
int rows = src.rows;
int cols = src.cols;

//compute new size
int rows_scaled = (int)(rows*size)/100;
int cols_scaled = (int)(cols*size)/100;

//display and resize
int flag = CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO;

//do the work
namedWindow(winName, flag);
imshow(winName, src);
resizeWindow(winName, cols_scaled, rows_scaled);
moveWindow(winName, pos_x, pos_y);

return cols_scaled;
}


//-----------------------------------------------------------
int imageStats(Mat& input, uint* stats, int chan_num){

//verify valid data type
CV_Assert(input.depth() == CV_8U);

int channels = input.channels();
int rows = input.rows;
int cols = input.cols * channels;

int cols_T = input.cols;
int rows_T = input.rows;

//test if image is stored in a continous memory space
if (input.isContinuous()){
    cols *= rows;
    rows  = 1;
}

int pixel_val = 0;
int max[3];
int min[3];
int avg[3];

switch(channels){

//grayscale image processing
case 1:{

    //pointer to read data
    uchar* p;

    max[0] = 0;
    min[0] = 255;
    avg[0] = 0;

    for(int i = 0; i < rows; ++i){

        p = input.ptr<uchar>(i);

        for (int j = 0; j < cols; ++j){

           pixel_val = p[j];
           avg[0] += pixel_val;
           if (min[0] > pixel_val) min[0] = pixel_val;
           if (max[0] < pixel_val) max[0] = pixel_val;
        }
    }

    stats[0] = min[0];
    stats[1] = max[0];
    stats[2] = avg[0]/(cols_T*rows_T);
} 

//color image processing
case 3:{ 

    //pointer to read data
    uchar* p;

    for(int i = 0; i < 3; i++){
        max[i] = 0;
        min[i] = 255;
        avg[i] = 0;
    }

    for(int i = 0; i < rows; ++i){

        p = input.ptr<uchar>(i);

        for (int j = 1; j < cols; j+=3 ){
           
           avg[0] += p[j-1];
           avg[1] += p[j];
           avg[2] += p[j+1];

           if (min[0] > p[j-1]) min[0] = p[j-1];
           if (min[1] > p[j]  ) min[1] = p[j];
           if (min[2] > p[j+1]) min[2] = p[j+1];

           if (max[0] < p[j-1]) max[0] = p[j-1];
           if (max[1] < p[j]  ) max[1] = p[j];
           if (max[2] < p[j+1]) max[2] = p[j+1];
        }

    }
 
for(int i= 0; i<channels; i++){
    stats[i]   = min[i];
    stats[i+3] = max[i];
    stats[i+6] = avg[i] / (cols_T*rows_T);
}
/*
    stats[0] = min[0];
    stats[1] = max[0];
    stats[2] = avg[0];
*/
}

}


/*
cout<<"MIN: "<< min<< endl;
cout<<"MAX: "<< max<< endl;
cout<<"AVG: "<< avg<< endl;
*/

return 0;
}
