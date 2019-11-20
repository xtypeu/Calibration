#pragma once

#include "src/common/calibrationdatabase.h"
#include "src/common/rectificationprocessor.h"

#include "world.h"

namespace slam {

class System
{
public:
    using FramePtr = World::FramePtr;

    using WorldPtr = World::WorldPtr;
    using WorldPointPtr = World::WorldPointPtr;

    System( const StereoCalibrationDataShort &calibration );
    System( const std::string &calibrationFile );

    std::list< FramePtr > &frames();
    const std::list< FramePtr > &frames() const;

    std::vector< WorldPointPtr > &worldPoints();
    const std::vector< WorldPointPtr > &worldPoints() const;

    CvImage keyPointsImage() const;
    CvImage stereoPointsImage() const;
    CvImage tracksImage() const;

    bool track( const std::string &leftFile, const std::string &rightFile );
    bool track( const CvImage &leftImage, const CvImage &rightImage );

protected:
    StereoRectificationProcessor m_rectificationProcessor;

    WorldPtr m_world;

private:
    void initialize();
};

}
