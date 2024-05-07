#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>

using namespace cv;


/**
 * Draw a ArUco marker of a specific ID at a specified location in img
 */
void drawMarker(cv::Mat& img, const int markerSize, const cv::Ptr<cv::aruco::Dictionary> dictionary, const unsigned int ID, const cv::Rect& ROIrange) {
    
    cv::Mat ROI;
    cv::Mat markerImage;
    
    cv::aruco::drawMarker(dictionary, ID, markerSize, markerImage, 1);
    cv::imwrite("marker" + std::to_string(ID) + ".png", markerImage);

    ROI = img(ROIrange);
    markerImage.copyTo(ROI);
    
    cv::Point textpos;
    cv::Size calibImageSz = img.size();
    ROI.locateROI(calibImageSz, textpos);
    textpos.x += 0;
    textpos.y +=markerSize + 30;
    cv::putText(img, std::to_string(ID), textpos, cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
}

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
    const int gridWidth = 15; // can do 45 at 4mm
    const int gridHeight = 8; // can do 45 at 4mm
    const int blockSize=static_cast<int>(dpmm * 10); // 20mm checkerbox
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
    
    
    // Generate some ArUco codes
    const int markerSize = 2 * blockSize;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    drawMarker(calibrationImage, markerSize, dictionary, 11, cv::Rect(markerSize, markerSize /*+ static_cast<int>(dpmm * 40)*/, markerSize, markerSize));
        
    drawMarker(calibrationImage, markerSize, dictionary, 12, cv::Rect(width - 2 * markerSize, markerSize /*+ static_cast<int>(dpmm * 40)*/, markerSize, markerSize));

    drawMarker(calibrationImage, markerSize, dictionary, 13, cv::Rect(width - 2 * markerSize, height - 2 * markerSize /*- static_cast<int>(dpmm * 40)*/, markerSize, markerSize));

    drawMarker(calibrationImage, markerSize, dictionary, 14, cv::Rect(markerSize, height - 2 * markerSize /*- static_cast<int>(dpmm * 40)*/, markerSize, markerSize));
    
    // Write combined image
    imwrite("calibration_target.png", calibrationImage);
    
    // Show the calibration target image
    std::string winname = "Calibration target";
    namedWindow(winname);
    imshow(winname, calibrationImage);
    waitKey();
    
    return 0;
}

