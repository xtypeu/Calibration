#pragma once

#include "src/common/iconswidget.h"

class QLabel;
class QBoxLayout;

class MonocularIcon;
class StereoIcon;

class CalibrationIconBase : public IconBase
{

public:
    using SuperClass = IconBase;

    CalibrationIconBase( const CvImage image, const cv::Size &frameSize, const QString &text );

    bool isMonocularIcon() const;
    bool isStereoIcon() const;

    MonocularIcon *toMonocularIcon();
    StereoIcon *toStereoIcon();

    const MonocularIcon *toMonocularIcon() const;
    const StereoIcon *toStereoIcon() const;

    void setFrameSize( const cv::Size &size );
    const cv::Size &frameSize() const;

protected:
    cv::Size m_frameSize;

private:
    void initialize();

};

class MonocularIcon : public CalibrationIconBase
{

public:
    using SuperClass = CalibrationIconBase;

    MonocularIcon(const CvImage previewImage, const cv::Size &frameSize, const std::vector< cv::Point2f > &points, const QString &text );

    void setPoints( const std::vector<cv::Point2f> &points );
    const std::vector< cv::Point2f > &points() const;

protected:
    std::vector< cv::Point2f > m_points;

private:
    void initialize();

};

class StereoIcon : public CalibrationIconBase
{

public:
    using SuperClass = CalibrationIconBase;

    StereoIcon( const CvImage leftPreviewImage, const CvImage rightPreviewImage, const cv::Size &frameSize,
                const std::vector< cv::Point2f > &leftPoints, const std::vector< cv::Point2f > &rightPoints, const QString &text );

    void setLeftPreview(const CvImage &image);
    void setRightPreview(const CvImage &image);

    const CvImage &leftPreview() const;
    const CvImage &rightPreview() const;

    const CvImage straightPreview() const;

    void setLeftPoints( const std::vector<cv::Point2f> &points );
    std::vector< cv::Point2f > leftPoints() const;

    void setRightPoints( const std::vector< cv::Point2f > &points );
    std::vector< cv::Point2f > rightPoints() const;

protected:
    CvImage m_leftPreview;
    CvImage m_rightPreview;

    std::vector<cv::Point2f> m_leftPoints;
    std::vector<cv::Point2f> m_rightPoints;

private:
    void initialize();

};

class CalibrationIconsWidget : public IconsListWidget
{
    Q_OBJECT

public:
    using SuperClass = IconsListWidget;

    explicit CalibrationIconsWidget( QWidget *parent = nullptr );

    void addIcon( CalibrationIconBase *icon );
    void insertIcon( CalibrationIconBase *icon );

    QList< CalibrationIconBase* > icons() const;

signals:
    void iconActivated( CalibrationIconBase *icon );

private:
    void initialize();

};