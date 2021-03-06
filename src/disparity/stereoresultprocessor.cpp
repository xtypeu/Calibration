#include "src/common/precompiled.h"

#include "stereoresultprocessor.h"

#include "src/common/functions.h"

// StereoResult
StereoResult::StereoResult()
{
    initialize();
}

void StereoResult::initialize()
{
}

void StereoResult::setPreviewImage( const CvImage &value )
{
    m_previewImage = value;
}

void StereoResult::setDisparity( const cv::Mat &value )
{
    m_disparity = value;
    m_colorizedDisparity = colorizeDisparity( m_disparity );
}

void StereoResult::setPoints( const cv::Mat &value )
{
    m_points = value;
}

void StereoResult::setPointCloud( const pcl::PointCloud< pcl::PointXYZRGB >::Ptr &value )
{
    m_pointCloud = value;
}

const CvImage &StereoResult::previewImage() const
{
    return m_previewImage;
}

const cv::Mat &StereoResult::disparity() const
{
    return m_disparity;
}

const CvImage &StereoResult::colorizedDisparity() const
{
    return m_colorizedDisparity;
}

const cv::Mat &StereoResult::points() const
{
    return m_points;
}

pcl::PointCloud< pcl::PointXYZRGB >::Ptr StereoResult::pointCloud() const
{
    return m_pointCloud;
}

void StereoResult::setFrame( const StampedStereoImage &frame )
{
    m_frame = frame;
}

const StampedStereoImage &StereoResult::frame() const
{
    return m_frame;
}

const StampedImage &StereoResult::leftFrame() const
{
    return m_frame.leftImage();
}

const StampedImage &StereoResult::rightFrame() const
{
    return m_frame.rightImage();
}

// StereoResultProcessor
StereoResultProcessor::StereoResultProcessor()
{
}

StereoResultProcessor::StereoResultProcessor( const std::shared_ptr< DisparityProcessorBase > &proc )
    : StereoProcessor( proc )
{
}

void StereoResultProcessor::setCalibration( const StereoCalibrationDataShort &data )
{
    auto calibration = data;

    m_rectificationProcessor.setCalibrationData( calibration );

    auto cropRect = calibration.cropRect();
    auto principal = cv::Vec2f( -cropRect.x, -cropRect.y );

    calibration.projectionMatrix().movePrincipalPoint( principal );

    setDisparityToDepthMatrix( calibration.disparityToDepthMatrix() );
}

bool StereoResultProcessor::loadYaml( const std::string &fileName )
{
    StereoCalibrationDataShort calibration;

    bool ret = calibration.loadYaml( fileName );

    if ( ret )
        setCalibration( calibration );

    return ret;
}

StereoResult StereoResultProcessor::process( const StampedStereoImage &frame )
{
    StereoResult ret;

    ret.setFrame( frame );

    if ( !frame.empty() ) {

        auto leftFrame = frame.leftImage();
        auto rightFrame = frame.rightImage();

        if ( m_rectificationProcessor.isValid() ) {

            CvImage leftRectifiedFrame;
            CvImage rightRectifiedFrame;

            CvImage leftCroppedFrame;
            CvImage rightCroppedFrame;

            m_rectificationProcessor.rectify( leftFrame, rightFrame, &leftRectifiedFrame, &rightRectifiedFrame );
            m_rectificationProcessor.crop( leftRectifiedFrame, rightRectifiedFrame, &leftCroppedFrame, &rightCroppedFrame );

            auto previewImage = stackImages( leftCroppedFrame, rightCroppedFrame );
            drawTraceLines( previewImage, 20 );

            ret.setPreviewImage( previewImage );

            if ( m_disparityProcessor ) {

                auto disparity = processDisparity( leftCroppedFrame, rightCroppedFrame );
                ret.setDisparity( disparity );

                cv::Mat points = reprojectPoints( disparity );
                ret.setPoints( points );

                auto pointCloud = producePointCloud( points, leftCroppedFrame );
                ret.setPointCloud( pointCloud );

            }

        }

    }

    return ret;

}

