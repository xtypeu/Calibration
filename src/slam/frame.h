#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

#include "framepoint.h"
#include "worldpoint.h"
#include "src/common/featureprocessor.h"

namespace slam {

class FrameBase
{
protected:
    FrameBase();
    virtual ~FrameBase() = default;
};

class MonoFrame : public FrameBase
{
public:
    using PointPtr = std::shared_ptr< MonoPoint >;

    virtual std::vector< PointPtr > framePoints() const = 0;

    std::vector< cv::Point2f > points() const;

    std::vector< StereoPoint > stereoPoints() const;
    std::vector< AdjacentPoint > adjacentPoints() const;

    bool drawPoints( CvImage *target ) const;

    void setCameraMatrix( const cv::Mat &value );
    const cv::Mat &cameraMatrix() const;

    void setRotation( const cv::Mat &value );
    const cv::Mat &rotation() const;

    void setTranslation( const cv::Mat &value );
    const cv::Mat &translation() const;

    void setProjectionMatrix( const cv::Mat &value );
    const cv::Mat &projectionMatrix() const;

protected:
    MonoFrame();

    cv::Mat m_cameraMatrix;

    cv::Mat m_r;
    cv::Mat m_t;

    mutable cv::Mat m_projectionMatrix;

private:
    void initialize();

};

class ProcessedFrame : public MonoFrame, public std::enable_shared_from_this< ProcessedFrame >
{
    friend class StereoFrame;
    friend class AdjacentFrame;
    friend class ProcessedPoint;

public:
    using FramePtr = std::shared_ptr< ProcessedFrame >;
    using FeaturePointPtr = std::shared_ptr< ProcessedPoint >;

    virtual std::vector< PointPtr > framePoints() const override;

    FeaturePointPtr &framePoint( const size_t index );
    const FeaturePointPtr &framePoint( const size_t index ) const;

    static FramePtr create();

    void load( const CvImage &image );

    const std::vector< cv::KeyPoint > &keyPoints() const;

    bool drawKeyPoints( CvImage *target ) const;

protected:
    ProcessedFrame();

    std::vector< cv::KeyPoint > m_keyPoints;
    std::vector< cv::Scalar > m_colors;
    cv::Mat m_descriptors;

    std::map< size_t, FeaturePointPtr > m_points;

    static FeatureProcessor m_processor;

    FeaturePointPtr createFramePoint( const size_t keyPointIndex, const cv::Scalar &color );

};

class Frame : public MonoFrame
{
};

class DoubleFrameBase : public FrameBase
{
public:
    using MonoFramePtr = std::shared_ptr< MonoFrame >;
    using MonoPointPtr = std::shared_ptr< MonoPoint >;

    void setFrames( const MonoFramePtr frame1, const MonoFramePtr frame2 );

    MonoFramePtr frame1() const;
    MonoFramePtr frame2() const;

protected:
    DoubleFrameBase();

    MonoFramePtr m_frame1;
    MonoFramePtr m_frame2;

    static FeatureMatcher m_matcher;

};

class StereoFrame : public DoubleFrameBase
{
public:
    using FeatureFramePtr = std::shared_ptr< ProcessedFrame >;
    using FramePtr = std::shared_ptr< StereoFrame >;

    static FramePtr create();

    void load( const CvImage &leftImage, const CvImage &rightImage );
    void matchFrames();

    MonoFramePtr leftFrame() const;
    MonoFramePtr rightFrame() const;

    std::vector< StereoPoint > stereoPoints() const;

    CvImage drawKeyPoints( const CvImage &leftImage, const CvImage &rightImage ) const;
    CvImage drawStereoPoints( const CvImage &leftImage, const CvImage &rightImage ) const;

protected:
    StereoFrame();

    static const int m_minLenght = 5;

};

class World;

class WorldStereoFrame : public StereoFrame
{
public:
    using WorldPtr = std::shared_ptr< World >;
    using FramePtr = std::shared_ptr< WorldStereoFrame >;

    static FramePtr create( const WorldPtr &parentWorld );

    const WorldPtr parentWorld() const;

    bool triangulatePoints();

protected:
    using WorldPtrImpl = std::weak_ptr< World >;

    WorldStereoFrame( const WorldPtr &parentWorld );

    WorldPtrImpl m_parentWorld;

};

class AdjacentFrame : public DoubleFrameBase
{
public:
    using FeatureFramePtr = std::shared_ptr< ProcessedFrame >;
    using FramePtr = std::shared_ptr< AdjacentFrame >;

    static FramePtr create();

    void load( const CvImage &prevImage, const CvImage &nextImage );
    void calcOpticalFlow( const CvImage &prevImage, const CvImage &nextImage );
    void matchFrames();

    bool track();

    MonoFramePtr previousFrame() const;
    MonoFramePtr nextFrame() const;

    std::vector< AdjacentPoint > adjacentPoints() const;

    CvImage drawTrack( const CvImage &image ) const;
    CvImage drawOpticalFrow( const CvImage &image ) const;

protected:    
    AdjacentFrame();

    std::map< size_t, cv::Point2f > m_opticalFlow;

    static const int m_minLenght = 5;
    static const int m_minPnpPoints = 50;

};

class WorldAdjacentFrame : public AdjacentFrame
{
public:
    using WorldPtr = std::shared_ptr< World >;
    using FramePtr = std::shared_ptr< WorldAdjacentFrame >;

    static FramePtr create( const WorldPtr &parentWorld );

    const WorldPtr parentWorld() const;

    bool triangulatePoints();

protected:
    using WorldPtrImpl = std::weak_ptr< World >;

    WorldAdjacentFrame( const WorldPtr &parentWorld );

    WorldPtrImpl m_parentWorld;

};

}
