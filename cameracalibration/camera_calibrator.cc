// This works with OpenCV 2 and above
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace cv;


int main() {
    //std::string img_path = "../../data/Gopro2/";
    //std::string img_path = "/Users/kimstp/Dropbox/DIKU_Images/DIKU_underwater/Seq3/Air_calibration_Narrow/Seq1/";
    
   // std::string img_path = "/Users/kimstp/Dropbox/DIKU_Images/DIKU_underwater/Seq4/Air_calibration_Linear/";
    
    std::string img_path = "/Users/kimstp/Documents/NHMD/data/Entomology/PinnedInsectTests/WORKPIOF0001_PyralidFOV/";

    std::vector<std::string> img_list;
    
    
    // iPhone
    /*img_list.push_back("IMG_0910.JPG");
    img_list.push_back("IMG_0911.JPG");
    img_list.push_back("IMG_0912.JPG");
    img_list.push_back("IMG_0913.JPG");
    img_list.push_back("IMG_0914.JPG");
     */
    /*
    img_list.push_back("IMG_0935.JPG");
    img_list.push_back("IMG_0936.JPG");
    img_list.push_back("IMG_0937.JPG");
    img_list.push_back("IMG_0938.JPG");
    img_list.push_back("IMG_0939.JPG");
    img_list.push_back("IMG_0940.JPG");
    img_list.push_back("IMG_0941.JPG");
    img_list.push_back("IMG_0942.JPG");
    */
    //img_list.push_back("GPTempDownload 21.JPG");
    //img_list.push_back("GPTempDownload 5.JPG");
    //img_list.push_back("GPTempDownload 6.JPG");
    
    
    /**
            * Following is for a 3x6 target with square length of 15mm
     */
    // Narrow view - does not work
    /*
    img_list.push_back("GOPR1358.JPG");
    img_list.push_back("GOPR1359.JPG");
    img_list.push_back("GOPR1360.JPG");
    img_list.push_back("GOPR1361.JPG");
    img_list.push_back("GOPR1362.JPG");
     */
    /*
    img_list.push_back("GOPR1363.JPG");
    img_list.push_back("GOPR1364.JPG");
    img_list.push_back("GOPR1365.JPG");
    img_list.push_back("GOPR1366.JPG");
    */
    
    // Wide view - works
    /*
    img_list.push_back("GOPR1367.JPG");
    img_list.push_back("GOPR1368.JPG");
    img_list.push_back("GOPR1369.JPG");
    img_list.push_back("GOPR1370.JPG");
    img_list.push_back("GOPR1371.JPG");
     */
    
    // Linear view - works
    /*
    img_list.push_back("GOPR1372.JPG");
    img_list.push_back("GOPR1373.JPG");
    img_list.push_back("GOPR1374.JPG");
    img_list.push_back("GOPR1375.JPG");
*/
    
    // Narrow view - does not work
    /*
    img_list.push_back("GOPR1376.JPG");
    img_list.push_back("GOPR1377.JPG");
    img_list.push_back("GOPR1378.JPG");
    img_list.push_back("GOPR1379.JPG");
    */
    
    /**
           * Following is for a 5x8 target with square length of 10mm
    */
    // Linear view - does not work
    /*
    img_list.push_back("GOPR1380.JPG");
    img_list.push_back("GOPR1381.JPG");
    img_list.push_back("GOPR1382.JPG");
    img_list.push_back("GOPR1383.JPG");
    */
    
    
    
    /**
           * Following is for a 3x10 target with square length of 15mm
    */
    // New waterproof target Linear view
    /*img_list.push_back("GOPR1387.JPG");
    img_list.push_back("GOPR1388.JPG");
    img_list.push_back("GOPR1389.JPG");
    img_list.push_back("GOPR1392.JPG");
    img_list.push_back("GOPR1393.JPG");
    img_list.push_back("GOPR1394.JPG");
    img_list.push_back("GOPR1395.JPG");
    img_list.push_back("GOPR1396.JPG");
     */
    /*img_list.push_back("GOPR1397.JPG");
    img_list.push_back("GOPR1398.JPG");
    img_list.push_back("GOPR1399.JPG");*/
    /*img_list.push_back("GOPR1400.JPG");
    img_list.push_back("G0011385.JPG");
    img_list.push_back("G0011386.JPG");
    img_list.push_back("G0021390.JPG");
    img_list.push_back("G0021391.JPG");
    */
    
    /**
     * Pinned insect a 7x10 target with square length of 10mm
     */
    img_list.push_back("Pyralidae_24_6115.png");
    
    /**
     * Pinned insect a 7x10 target with square length of 1mm
     */
    //img_list.push_back("Pyralidae_24_6114.png");
    
    std::string winname = "Calibration image";
    namedWindow(winname, WINDOW_NORMAL);
    
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);;
    Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat > rvecs, tvecs;
    
    Mat img, gray;

    std::vector<Point2f> corners;
    std::vector<std::vector<Point2f> > corners_list;
    
    Size patternSize(7,10); // Number of internal corner points in the calibration checkerboard pattern
    int patternUnit = 1; // measured in mm
    
    for (int i = 0; i < img_list.size(); i++) {
        img = imread(img_path + img_list[i]);
        cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to gray scale
        

        //bool patternFound = findChessboardCorners(gray, patternSize, corners, CALIB_CB_FAST_CHECK + CALIB_CB_ADAPTIVE_THRESH);
        //bool patternFound = findChessboardCorners(gray, patternSize, corners, CALIB_CB_FILTER_QUADS + CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
        //bool patternFound = findChessboardCorners(gray, patternSize, corners, CALIB_CB_FAST_CHECK);
        bool patternFound = findChessboardCornersSB(gray, patternSize, corners, CALIB_CB_EXHAUSTIVE);
        if (patternFound) {
            cornerSubPix(gray, corners, Size(5,5), Size(-1,-1), TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
        
            drawChessboardCorners(img, patternSize, corners, patternFound);
            
            std::cout << img_list[i] << std::endl;
            
        } else {
            std::cout << "No pattern found!" << std::endl;
        }
        
        imshow(winname, img);
        
        
        corners_list.push_back(corners);
        
        
        waitKey(); // Wait forever for a key press
    }
    
    
    // Create a list of checkerboard corner points in object 3D coordinates
    std::vector<Point3f> patternPoints;
    
    for (int i = 0; i < patternSize.height; i++) {
        for (int j = 0; j < patternSize.width; j++) {
            Point3f pPoint(j * patternUnit, i * patternUnit, 0.0f);
            patternPoints.push_back(pPoint);
        }
    }
    
    std::vector<std::vector<Point3f> > patterns_list;
    
    // Populate the patterns list by duplicating as many times as we have images
    for (int i = 0; i < corners_list.size(); i++)
        patterns_list.push_back(patternPoints);
    
    
    
    // Calibrate camera instrinsic parameters based on analysed images
    Size imgSize(img.cols, img.rows);
    double rms = calibrateCamera(patterns_list, corners_list, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs);
    //double rms = calibrateCamera(patterns_list, corners_list, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K1 + CALIB_FIX_K2 + CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5 + CALIB_FIX_K6 + CALIB_FIX_S1_S2_S3_S4);
    
    std::cout << "Calibration reprojection error = " << rms << std::endl;
    
    
    // Save results in YAML format
    FileStorage fs( "calibration_params.yml", FileStorage::WRITE );
    
    fs << "ImageSize" << imgSize;
    fs << "reprojection_error" << rms;
    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;
    
    
    
    return 0;
}
