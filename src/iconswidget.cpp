#include "precompiled.h"

#include "iconswidget.h"
#include "defs.h"

// IconBase
IconBase::IconBase( QWidget* parent )
    : ImageWidget( parent )
{
    initialize();
}

IconBase::IconBase(const CvImage frame, QWidget* parent )
    : ImageWidget( frame, parent )
{
    initialize();
}

void IconBase::initialize()
{
    setMouseTracking( true );
    setAttribute( Qt::WA_Hover );
}

void IconBase::setPreviewImage( const CvImage &image )
{
    ImageWidget::setImage( image );
}

const CvImage IconBase::previewImage() const
{
    return ImageWidget::image();
}

void IconBase::paintEvent( QPaintEvent *event )
{
    ImageWidget::paintEvent( event );

    if ( underMouse() ) {

        QStylePainter painter(this);

        painter.setRenderHint( QPainter::HighQualityAntialiasing );

        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().color( QPalette::Highlight );

        painter.drawPrimitive( QStyle::PE_Frame, option );

    }

}

void IconBase::mouseDoubleClickEvent( QMouseEvent */*event*/ )
{
    emit iconActivated( this );
}

// MonocularIcon
MonocularIcon::MonocularIcon(const CvImage previewImage, const CvImage sourceImage, QWidget* parent )
    : IconBase( previewImage, parent )
{
    setSourceImage( sourceImage );

    initialize();
}

void MonocularIcon::initialize()
{
}

void MonocularIcon::setSourceImage( const CvImage &image )
{
    m_sourceImage = image;
}

const CvImage MonocularIcon::sourceImage() const
{
    return m_sourceImage;
}

// StereoIcon
StereoIcon::StereoIcon( const CvImage previewImage, const CvImage leftSourceImage, const CvImage rightSourceImage, QWidget* parent )
    : IconBase( previewImage, parent )
{
    setLeftSourceImage( leftSourceImage );
    setRightSourceImage( rightSourceImage );

    initialize();
}

void StereoIcon::initialize()
{
}

void StereoIcon::setLeftSourceImage( const CvImage &image )
{
    m_leftSourceImage = image;
}

void StereoIcon::setRightSourceImage( const CvImage &image )
{
    m_rightSourceImage = image;
}

const CvImage StereoIcon::leftSourceImage() const
{
    return m_leftSourceImage;
}

const CvImage StereoIcon::rightSourceImage() const
{
    return m_rightSourceImage;
}


// FrameIconsWidget
IconsLayout::IconsLayout( QWidget *parent )
    : QWidget( parent )
{
    initialize();
}

void IconsLayout::initialize()
{
    m_layout = new QBoxLayout( QBoxLayout::LeftToRight, this );

    setLayout( m_layout );

}

void IconsLayout::setOrientation( const Qt::Orientation value )
{
    if ( value == Qt::Horizontal )
        m_layout->setDirection( QBoxLayout::LeftToRight );
    else
        m_layout->setDirection( QBoxLayout::TopToBottom );

}

Qt::Orientation IconsLayout::orientation() const
{
    if ( m_layout->direction() == QBoxLayout::LeftToRight || m_layout->direction() == QBoxLayout::RightToLeft )
        return Qt::Horizontal;
    else
        return Qt::Vertical;
}

void IconsLayout::insertIcon( IconBase *icon )
{
    if ( icon ) {

        connect( icon, &IconBase::iconActivated, this, &IconsLayout::iconActivated );

        m_layout->insertWidget( 0, icon );

    }

}

void IconsLayout::addIcon( IconBase *icon )
{
    if ( icon ) {

        connect( icon, &IconBase::iconActivated, this, &IconsLayout::iconActivated );

        m_layout->addWidget( icon );

    }

}

unsigned int IconsLayout::iconsCount() const
{
    return m_layout->count();
}

double IconsLayout::maximumAspectRatio() const
{
    double ret = 0.0;

    for ( auto i = 0; i < m_layout->count(); ++i ) {
        auto frame = iconAt( i );

        if (frame) {
            auto aspect = frame->imageAspect();

            ret = std::max( 0.0, aspect );
        }
    }

    return ret;

}

IconBase *IconsLayout::iconAt( const size_t i ) const
{
    return dynamic_cast< IconBase * > ( m_layout->itemAt( i )->widget() );
}

void IconsLayout::clear()
{
    QLayoutItem *child;

    while ( ( child = m_layout->takeAt( 0 ) ) != nullptr ) {
        delete child->widget();
        delete child;
    }

}

// FrameIconsWidget
IconsWidget::IconsWidget( QWidget *parent )
    : QScrollArea( parent )
{
    initialize();

}

void IconsWidget::initialize()
{
    auto palette = this->palette();
    palette.setColor( QPalette::Background, Qt::white );
    setPalette( palette );

    setAutoFillBackground( true );

    auto layout = new IconsLayout( this );
    setWidget( layout );

    connect( layout, &IconsLayout::iconActivated, this, &IconsWidget::iconActivated );

    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

}

IconsLayout *IconsWidget::layoutWidget() const
{
    return dynamic_cast<IconsLayout *>( widget() );
}

void IconsWidget::addIcon(IconBase *icon)
{
    auto layout = layoutWidget();

    layout->addIcon( icon );

    updateLayout();
}

void IconsWidget::insertIcon(IconBase *icon)
{
    auto layout = layoutWidget();

    layout->insertIcon( icon );

    updateLayout();
}

void IconsWidget::setOrientation( const Qt::Orientation value )
{
    layoutWidget()->setOrientation( value );

    updateLayout();
}

Qt::Orientation IconsWidget::orientation() const
{
    return layoutWidget()->orientation();
}

void IconsWidget::updateLayout()
{
    auto layout = layoutWidget();

    int width;
    int height;

    if ( orientation() == Qt::Horizontal ) {
        height = viewport()->height();
        width = layout->iconsCount() * layout->maximumAspectRatio() * height;

    }
    else {

        width = viewport()->width();

        if ( layout->maximumAspectRatio() > DOUBLE_EPS )
            height = layout->iconsCount() * width / layout->maximumAspectRatio();
        else
            height = 0;

    }

    layout->setMinimumSize( width, height );
    layout->setMaximumSize( width, height );

}

void IconsWidget::resizeEvent(QResizeEvent */*event*/)
{
    updateLayout();
}
