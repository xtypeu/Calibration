#include "src/common/precompiled.h"

#include "mainwindow.h"

#include "disparitypreviewwidget.h"
#include "documentwidget.h"
#include "src/common/ipwidget.h"

MainWindow::MainWindow( QWidget *parent )
    : MainWindowBase( parent )
{
    initialize();
}

MainWindow::MainWindow( const QString &leftCameraIp, const QString &rightCameraIp, QWidget *parent )
    : MainWindowBase( parent )
{
    initialize();

    addCameraDisparityDocument( leftCameraIp, rightCameraIp );
}

void MainWindow::initialize()
{
    setupActions();
    setupMenus();
    setupToolBars();
}

void MainWindow::addImageDisparityDocument()
{
    addDocument( new ImageDisparityDocument( this ) );
}

void MainWindow::addCameraDisparityDocument( const QString &leftCameraIp, const QString &rightCameraIp )
{
    addDocument( new CameraDisparityDocument( leftCameraIp, rightCameraIp, this ) );
}

CameraDisparityDocument *MainWindow::currentCameraDisparityDocument() const
{
    return getCurrentDocument< CameraDisparityDocument >();
}

void MainWindow::loadCalibrationFile( const QString &fileName )
{
    auto doc = currentCameraDisparityDocument();

    if ( doc )
        doc->loadCalibrationFile( fileName );
}

void MainWindow::addImageDisparity()
{
    addImageDisparityDocument();
}

void MainWindow::addCameraDisparityDialog()
{
    StereoIPDialog dialog( this );

    if ( dialog.exec() == DialogBase::Accepted )
        addCameraDisparityDocument( dialog.leftIp(), dialog.rightIp() );

}

void MainWindow::loadCalibrationDialog()
{
    auto file = QFileDialog::getOpenFileName(
                            this,
                            tr( "Select calibration file" ),
                            QString(),
                            "Calibration files (*.yaml)" );

    if ( !file.isEmpty() )
        loadCalibrationFile( file );

}

void MainWindow::importDialog()
{
}

void MainWindow::exportDialog()
{
}

void MainWindow::clearIcons()
{
}

void MainWindow::settingsDialog()
{
}

void MainWindow::setupActions()
{
    m_newImageDocumentAction = new QAction( QIcon( ":/resources/images/new.ico" ), tr( "New image disparity" ), this );
    m_newCameraDocumentAction = new QAction( QIcon( ":/resources/images/new.ico" ), tr( "New camera disparity" ), this );
    m_loadCalibrationAction = new QAction( QIcon( ":/resources/images/open.ico" ), tr( "Load calibration file" ), this );

    m_importAction = new QAction( QIcon( ":/resources/images/export.ico" ), tr( "Import" ), this );
    m_exportAction = new QAction( QIcon( ":/resources/images/import.ico" ), tr( "Export" ), this );

    m_clearIconsAction = new QAction( QIcon( ":/resources/images/trash.ico" ), tr( "Clear" ), this );

    m_settingsAction = new QAction( QIcon( ":/resources/images/settings.ico" ), tr( "Settings" ), this );
    m_exitAction = new QAction( QIcon( ":/resources/images/power.ico" ), tr( "Exit" ), this );
    m_aboutAction = new QAction( QIcon( ":/resources/images/help.ico" ), tr( "About" ), this );

    connect( m_newImageDocumentAction, &QAction::triggered, this, &MainWindow::addImageDisparity );
    connect( m_newCameraDocumentAction, &QAction::triggered, this, &MainWindow::addCameraDisparityDialog );

    connect( m_loadCalibrationAction, &QAction::triggered, this, &MainWindow::loadCalibrationDialog );

    connect( m_importAction, &QAction::triggered, this, &MainWindow::importDialog );
    connect( m_exportAction, &QAction::triggered, this, &MainWindow::exportDialog );

    connect( m_clearIconsAction, &QAction::triggered, this, &MainWindow::clearIcons );
    connect( m_settingsAction, &QAction::triggered, this, &MainWindow::settingsDialog );
    connect( m_exitAction, &QAction::triggered, this, &MainWindow::close );

}

void MainWindow::setupMenus()
{
    m_menuBar = new QMenuBar(this);

    auto fileMenu = m_menuBar->addMenu( tr( "File" ) );
    fileMenu->addAction( m_newImageDocumentAction );
    fileMenu->addAction( m_newCameraDocumentAction );
    fileMenu->addSeparator();
    fileMenu->addAction( m_loadCalibrationAction );
    fileMenu->addSeparator();
    fileMenu->addAction( m_importAction );
    fileMenu->addAction( m_exportAction );
    fileMenu->addSeparator();
    fileMenu->addAction( m_exitAction );

    auto actionsMenu = m_menuBar->addMenu( tr( "Actions" ) );
    actionsMenu->addAction( m_clearIconsAction );
    actionsMenu->addSeparator();
    actionsMenu->addAction( m_settingsAction );

    auto helpMenu = m_menuBar->addMenu( tr( "Help" ) );
    helpMenu->addAction( m_aboutAction );

    setMenuBar(m_menuBar);

}

void MainWindow::setupToolBars()
{
    // Настройки панели инструментов проекта
    m_toolBar = new QToolBar( tr( "Project tool bar" ), this );
    addToolBar( m_toolBar );

    m_toolBar->addAction( m_newImageDocumentAction );
    m_toolBar->addAction( m_newCameraDocumentAction );
    m_toolBar->addSeparator();
    m_toolBar->addAction( m_loadCalibrationAction );

}
