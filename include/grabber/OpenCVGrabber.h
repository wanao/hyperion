#pragma once

// Qt includes
#include <QObject>

// util includes
#include <utils/Image.h>
#include <utils/ColorRgb.h>

// grabber includes
#include <opencv2/opencv.hpp>

#include <vector>

/// Capture class for OpenCV
class OpenCVGrabber : public QObject
{
    Q_OBJECT

public:
    OpenCVGrabber(int input, int width, int height);
    void setTransform(bool hFlip, bool vFlip,int width, int height, std::vector<std::pair<float,float>> & persp);
    virtual ~OpenCVGrabber();

public slots:
    void grabFrame(Image<ColorRgb> & image);

private:
    cv::VideoCapture _capture;

    cv::Mat _transform;
};
