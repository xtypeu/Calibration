#pragma once

#include <QString>
#include <QImage>

#include <opencv2/opencv.hpp>

class CvImage;

class QtImage : public QImage
{
public:
    QtImage();
    QtImage( const QSize &size, Format format = Format_RGBA8888 );
    QtImage( int width, int height, Format format = Format_RGBA8888 );
    QtImage( const QImage &img );
    QtImage( const CvImage &img );
};

class CvImage : public cv::Mat
{
public:
    CvImage();
    CvImage( int width, int height, int type );
    CvImage( cv::Size size, int type );
    CvImage( int width, int height, int type, const cv::Scalar& color );
    CvImage( cv::Size size, int type, const cv::Scalar& color );
    CvImage( int rows, int cols, int type, void* data, size_t step=AUTO_STEP );
    CvImage( const cv::Mat &mat );
    CvImage( const QtImage &img );

    int width() const;
    int height() const;
    double aspectRatio() const;
    double revAspectRatio() const;

};