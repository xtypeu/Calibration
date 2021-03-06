#include "src/common/precompiled.h"

#include "slamdocument.h"

#include "slamwidget.h"

// DisparityDocumentBase
SlamDocumentBase::SlamDocumentBase( QWidget* parent )
    : DocumentBase( parent )
{
    initialize();
}

void SlamDocumentBase::initialize()
{
}

// ImageSlamDocument
ImageSlamDocument::ImageSlamDocument( const QStringList &leftList, const QStringList &rightList, const QString &calibrationFile, QWidget* parent )
    : SlamDocumentBase( parent )
{
    initialize( leftList, rightList, calibrationFile );
}

void ImageSlamDocument::initialize( const QStringList &leftList, const QStringList &rightList, const QString &calibrationFile )
{
    setWidget( new SlamImageWidget( leftList, rightList, calibrationFile, this ) );
}

SlamImageWidget *ImageSlamDocument::widget() const
{
    return dynamic_cast< SlamImageWidget * >( m_widget );
}

// CameraSlamDocument
CameraSlamDocument::CameraSlamDocument( const QString &leftCameraIp, const QString &rightCameraIp, const QString &calibrationFile, QWidget* parent )
    : SlamDocumentBase( parent )
{
    initialize( leftCameraIp, rightCameraIp, calibrationFile );
}

void CameraSlamDocument::initialize( const QString &leftCameraIp, const QString &rightCameraIp, const QString &calibrationFile )
{
    setWidget( new SlamCameraWidget( leftCameraIp, rightCameraIp, calibrationFile, this ) );
}

SlamCameraWidget *CameraSlamDocument::widget() const
{
    return dynamic_cast< SlamCameraWidget * >( m_widget );
}

// ImuDocument
ImuDocument::ImuDocument( const QString &portName, QWidget* parent )
    : SlamDocumentBase( parent )
{
    initialize( portName );
}

ImuViewWidget *ImuDocument::widget() const
{
    return dynamic_cast< ImuViewWidget * >( m_widget );
}

void ImuDocument::initialize( const QString &portName )
{
    setWidget( new ImuViewWidget( portName, this ) );
}
