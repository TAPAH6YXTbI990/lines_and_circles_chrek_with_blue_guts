#include <opencv2/opencv.hpp>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace cv;

int main()
{
    std::system("chcp 65001 > nul");

    std::string image_path = "C:/Users/User/.vscode/projects/open_guts.jpg"; 
   
    Mat img = imread(image_path);

    if (img.empty())
    {
        std::cerr << "Îøèáêà ïðè çàãðóçêå èçîáðàæåíèÿ!" << std::endl;
        return -1; 
    }

    int down_width = 500;
    int down_height = 500;

    resize(img, img, Size(down_width, down_height), INTER_LINEAR); 

    Mat grayImg, hsvImg, labImg, xyzImg, edges, mask, result;

    cvtColor(img, grayImg, COLOR_BGR2GRAY);
    cvtColor(img, hsvImg, COLOR_BGR2HSV);
    cvtColor(img, labImg, COLOR_BGR2Lab);
    cvtColor(img, xyzImg, COLOR_BGR2XYZ); 
    
    Canny(grayImg, edges, 50, 150, 3);
    GaussianBlur(grayImg, grayImg, Size(9,9), 2, 2);
    
    std::vector<Vec2f> lines; 
    HoughLines(edges, lines,1, CV_PI/5, 200); 

    for (size_t i = 0; i < lines.size();i++) { 
        float rho = lines[i][0]; 
        float theta = lines[i][1]; 
        double a = cos(theta), b = sin(theta); 
        double x0 = a * rho, y0 = b * rho; 
        Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a))); 
        Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a))); 
        line(img, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA); 
    } 
    
    std::vector<Vec3f> circles; 
    HoughCircles(grayImg, circles, HOUGH_GRADIENT, 1, grayImg.rows / 8, 100, 30, 30, 100); 

    for (size_t i = 0; i < circles.size(); i++) { 
        Vec3i c = circles[i]; 
        Point center(c[0], c[1]); 
        int radius = c[2]; 
        circle(img, center, 3, Scalar(0, 255, 0), -1, LINE_AA); 
        circle(img, center, radius, Scalar(255, 0, 0), 2, LINE_AA); 
    }
    

    inRange(hsvImg, Scalar(100, 150, 0), Scalar(140, 255, 255), mask);

    bitwise_and(img, img, result, mask);
    
    imshow("OrigImg", img); 
    imshow("Mask", mask);
    imshow("Result", result); 
    imshow("Edges", edges);
    imshow("GrayImg", grayImg);
    imshow("HSVimg", hsvImg);
    imshow("LabImg", labImg);
    imshow("xyzImg", xyzImg);

    int k = waitKey(0); 

    return 0;
}
