#include <iostream>
#include <sstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>

#include "grabber/OpenCVGrabber.h"


OpenCVGrabber::OpenCVGrabber(int input, int width, int height)
    : _capture(input)
{
    if (width && height) {
        _capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
        _capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }
}

void OpenCVGrabber::setTransform(bool hFlip, bool vFlip,int width, int height, std::vector<std::pair<float,float>> & persp)
{
    std::vector< cv::Point2f> origin(4);

    std::vector< cv::Point2f> new_persp = {{0,0}, {(float)width,0}, {0,(float)height}, {(float)width,(float)height}};

    assert(persp.size() == 4);

    for (int i=0;i<4;i++){
        origin[i].x = persp[i].first;
        origin[i].y = persp[i].second;
    }

    if (hFlip) {
        swap(new_persp[0],new_persp[1]);
        swap(new_persp[2],new_persp[3]);
    }

    if (vFlip) {
        swap(new_persp[0],new_persp[2]);
        swap(new_persp[1],new_persp[3]);
    }

    _transform = cv::getPerspectiveTransform(origin, new_persp);
}

OpenCVGrabber::~OpenCVGrabber()
{
}

void OpenCVGrabber::grabFrame(Image<ColorRgb> & image)
{
    cv::Mat frame;
    _capture >> frame;

    const int width = frame.cols, height = frame.rows;

    // apply transformation
    if (_transform.total() > 0) {
        cv::Mat tmp;

        warpPerspective(frame, tmp, _transform, cv::Size(width,height));

        frame = tmp;
    }

    cv::Mat rgbFrame(width, height, CV_8UC3);
    cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    image.resize(width, height);
    memcpy(image.memptr(), rgbFrame.ptr(), width * height * 3);
}
