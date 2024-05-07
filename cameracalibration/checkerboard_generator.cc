#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main() {

    // Define some relevant constants
    const float dpi = 72; // dots (pixels) per inch [inch^(-1)]
    const float inch2mm = 25.4; // [mm / inch]
    const float dpmm = dpi / inch2mm; // Dots per mm [mm^(-1)]

    // Define size of output image to fit A4 paper
    const float a4width = 210.0f; // [mm]
    const float a4height = 297.0f; // [mm]
    const float a4aspectratio =   a4width / a4height; // A4 paper aspect

    // Size of output image
    const int width = static_cast<int>(a4width * dpmm);
    //const int height = static_cast<int>(width * a4aspectratio); // [pixels]
    const int height = static_cast<int>(a4height * dpmm); // [pixels]
    
    // Create a chessboard pattern
    const int blockSizeinMM = 15; // mm
    const int gridWidth = 11; // can do 45 at 4mm
    const int gridHeight = 15; // can do 45 at 4mm
    const int blockSize=static_cast<int>(dpmm * blockSizeinMM); // checkerbox
    const int chessboardWidth=blockSize * gridWidth;
    const int chessboardHeight=blockSize * gridHeight;
    Mat chessBoard(cv::Size(chessboardWidth, chessboardHeight),CV_8UC1,Scalar::all(0));
    unsigned char color=0;
    
    for(int i=0;i<chessboardHeight;i=i+blockSize){
        color=~color;
        for(int j=0;j<chessboardWidth;j=j+blockSize){
            Mat ROI=chessBoard(Rect(j,i,blockSize,blockSize));
            ROI.setTo(Scalar::all(color));
            color=~color;
        }
        color=~color;
    }
        
    
    // Create combined image
    cv::Mat calibrationImage(cv::Size(width, height), CV_8UC1, Scalar::all(255));
    
    cv::Mat chessROI = calibrationImage(cv::Rect(width / 2 - chessboardWidth / 2, height / 2 - chessboardHeight / 2, chessboardWidth, chessboardHeight));
    chessBoard.copyTo(chessROI);
    
    
    // Write combined image
    imwrite("calibration_target.png", calibrationImage);
    
    // Show the calibration target image
    std::string winname = "Calibration target";
    namedWindow(winname);
    imshow(winname, calibrationImage);
    waitKey();
    
    return 0;

}
