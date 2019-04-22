#pragma once

#include <QWidget>
#include <QPointer>

#include "image.h"

#include "templateprocessor.h"

#include "camerawidget.h"

class QVBoxLayout;

class ParametersWidget;

class TaskWidgetBase : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidgetBase( QWidget* parent = nullptr );

    TemplateProcessor::Type templateType() const;
    const cv::Size &templateCount() const;
    double templateSize() const;
    bool resizeFlag() const;
    unsigned int frameMaximumFlag() const;

    bool adaptiveThreshold() const;
    bool normalizeImage() const;
    bool filterQuads() const;
    bool fastCheck() const;

    void setCameraDecimation( CameraWidgetBase::DecimationType type );

protected slots:
    void updateParameters();

protected:
    QPointer<QVBoxLayout> m_layout;
    QPointer<ParametersWidget> m_parametersWidget;
    QPointer<CameraWidgetBase> m_cameraWidget;

private:
    void initialize();

};

class MonocularTaskWidget : public TaskWidgetBase
{
    Q_OBJECT

public:
    explicit MonocularTaskWidget( const std::string &cameraIp, QWidget* parent = nullptr );

    MonocularCameraWidget *cameraWidget() const;

    const CvImage sourceImage() const;
    const CvImage previewImage() const;

    bool isTemplateExist() const;

private:
    void initialize ( const std::string &cameraIp );

};


class StereoTaskWidget : public TaskWidgetBase
{
    Q_OBJECT

public:
    explicit StereoTaskWidget( const std::string &leftCameraIp, const std::string &rightCameraIp, QWidget* parent = nullptr );

    StereoCameraWidget *cameraWidget() const;

    const CvImage leftSourceImage() const;
    const CvImage leftDisplayedImage() const;

    const CvImage rightSourceImage() const;
    const CvImage rightDisplayedImage() const;

    bool isTemplateExist() const;

public slots:
    void setLeftSourceImage( const CvImage image );
    void setLeftDisplayedImage( const CvImage image );

    void setRightSourceImage( const CvImage image );
    void setRightDisplayedImage( const CvImage image );

private:
    void initialize(const std::string &leftCameraIp, const std::string &rightCameraIp );

};


