#pragma once

#include <QWidget>
#include <QPointer>

class IntSliderBox;
class DoubleSliderBox;

class DisparityControlWidget : public QWidget
{
    Q_OBJECT

public:
    DisparityControlWidget( QWidget* parent = nullptr );

    int prefilterSize() const;
    int prefilterCap() const;
    int sadWindowSize() const;
    int minDisparity() const;
    int numDisparities() const;
    int textureThreshold() const;
    int uniquessRatio() const;
    int speckleWindowSize() const;
    int speckleRange() const;

public slots:
    void setPrefilterSize( const int value );
    void setPrefilterCap( const int value );
    void setSadWindowSize( const int value );
    void setMinDisparity( const int value );
    void setNumDisparities( const int value );
    void setTextureThreshold( const int value );
    void setUniquessRatio( const int value );
    void setSpeckleWindowSize( const int value );
    void setSpeckleRange( const int value );

protected:
    QPointer< IntSliderBox > m_preFilterSizeBox;
    QPointer< IntSliderBox > m_preFilterCapBox;
    QPointer< IntSliderBox > m_sadWindowSizeBox;
    QPointer< IntSliderBox > m_minDisparityBox;
    QPointer< IntSliderBox > m_numDisparitiesBox;
    QPointer< IntSliderBox > m_textureThresholdBox;
    QPointer< IntSliderBox > m_uniquessRatioBox;
    QPointer< IntSliderBox > m_speckleWindowSizeBox;
    QPointer< IntSliderBox > m_speckleRangeBox;

private:
    void initialize();

};