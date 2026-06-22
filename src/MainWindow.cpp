#include "MainWindow.h"
#include "ImageViewer.h"
#include "ImageLoader.h"
#include "ImageProcessor.h"

#include <QImage>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QStatusBar>
#include <QKeySequence>
#include <QSettings>
#include <QCloseEvent>
#include <QToolBar>

namespace
{
  constexpr int kMaxRecentFiles = 5;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Qt Viewer");
  resize(1000, 700);

  createViewer();
  createMenus();
  createToolBar();
  createStatusBar();
  loadSettings();
}

void MainWindow::saveImageAs()
{
  const QImage image = viewer_->image();

  if (image.isNull()) {
    QMessageBox::information(this,
                             "No Image",
                             "There is no image to save.");
    return;
  }

  const QString fileName = QFileDialog::getSaveFileName(
      this,
      "Save Image As",
      "processed_image.png",
      "Images (*.png *.jpg *.jpeg *.bmp)");

  if (fileName.isEmpty())
    return;

  if (!image.save(fileName)) {
    QMessageBox::warning(this,
                         "Save Failed",
                         "Could not save the image.");
  }
}

void MainWindow::openImage()
{
  const QString fileName = QFileDialog::getOpenFileName(
      this,
      "Open Image",
      QString(),
      "Images (*.png *.jpg *.jpeg *.bmp)"
      );

  if (fileName.isEmpty())
    return;

  openImage(fileName);
}

void MainWindow::openImage(const QString& fileName)
{
  ImageLoader loader;

  QImage image = loader.load(fileName);

  if (image.isNull())
  {
    QMessageBox::warning(
        this,
        "Error",
        "Failed to load image."
        );
    return;
  }

  viewer_->setImage(image);
  updateStatusBar();

  addRecentFile(fileName);
}

void MainWindow::fitToWindow()
{
  viewer_->fitToWindow();
  updateStatusBar();
}

void MainWindow::actualSize()
{
  viewer_->actualSize();
  updateStatusBar();
}

void MainWindow::createViewer()
{
  viewer_ = new ImageViewer(this);
  setCentralWidget(viewer_);

  // Explicitly select openImage(const QString&) because openImage() is overloaded.
  connect(viewer_,
          &ImageViewer::imageDropped,
          this,
          static_cast<void (MainWindow::*)(const QString&)>(
              &MainWindow::openImage));
}

void MainWindow::createMenus()
{
  createFileMenu();
  createViewMenu();
  createImageMenu();
}

void MainWindow::createFileMenu()
{
  QMenu *fileMenu = menuBar()->addMenu("&File");

  openAction_ = fileMenu->addAction("&Open Image...");
  openAction_->setShortcut(QKeySequence::Open);
  openAction_->setStatusTip("Open an image file");

  saveAsAction_ = fileMenu->addAction("Save Image &As...");
  saveAsAction_->setShortcut(QKeySequence::SaveAs);
  saveAsAction_->setStatusTip("Save the current image as a file");
  connect(saveAsAction_,
          &QAction::triggered,
          this,
          &MainWindow::saveImageAs);

  recentMenu_ = fileMenu->addMenu("Open &Recent");
  fileMenu->addSeparator();

  // Explicitly select openImage() because it is overloaded.
  connect(openAction_,
          &QAction::triggered,
          this,
          static_cast<void (MainWindow::*)()>(&MainWindow::openImage));
}

void MainWindow::createViewMenu()
{
  QMenu *viewMenu = menuBar()->addMenu("&View");

  zoomInAction_ = viewMenu->addAction("Zoom &In");
  zoomInAction_->setShortcut(QKeySequence::ZoomIn);
  zoomInAction_->setStatusTip("Zoom in");
  connect(zoomInAction_, &QAction::triggered,
          this, &MainWindow::zoomIn);

  zoomOutAction_ = viewMenu->addAction("Zoom &Out");
  zoomOutAction_->setShortcut(QKeySequence::ZoomOut);
  zoomOutAction_->setStatusTip("Zoom out");
  connect(zoomOutAction_, &QAction::triggered,
          this, &MainWindow::zoomOut);

  viewMenu->addSeparator();

  fitAction_ = viewMenu->addAction("Fit to &Window");
  fitAction_->setShortcut(QKeySequence("Ctrl+F"));
  fitAction_->setStatusTip("Fit image to window");
  connect(fitAction_, &QAction::triggered,
          this, &MainWindow::fitToWindow);

  actualSizeAction_ = viewMenu->addAction("&Actual Size");
  actualSizeAction_->setShortcut(QKeySequence("Ctrl+0"));
  actualSizeAction_->setStatusTip("Show image at actual size");
  connect(actualSizeAction_, &QAction::triggered,
          this, &MainWindow::actualSize);
}

void MainWindow::createImageMenu()
{
  QMenu *imageMenu = menuBar()->addMenu("&Image");

  rotateLeftAction_ = imageMenu->addAction("Rotate &Left");
  rotateLeftAction_->setShortcut(QKeySequence("Ctrl+L"));
  rotateLeftAction_->setStatusTip("Rotate image left");
  connect(rotateLeftAction_, &QAction::triggered,
          this, &MainWindow::rotateLeft);

  rotateRightAction_ = imageMenu->addAction("Rotate &Right");
  rotateRightAction_->setShortcut(QKeySequence("Ctrl+R"));
  rotateRightAction_->setStatusTip("Rotate image right");
  connect(rotateRightAction_, &QAction::triggered,
          this, &MainWindow::rotateRight);

  imageMenu->addSeparator();

  flipHorizontalAction_ = imageMenu->addAction("Flip &Horizontal");
  flipHorizontalAction_->setStatusTip("Flip image horizontally");
  connect(flipHorizontalAction_, &QAction::triggered,
          this, &MainWindow::flipHorizontal);

  flipVerticalAction_ = imageMenu->addAction("Flip &Vertical");
  flipVerticalAction_->setStatusTip("Flip image vertically");
  connect(flipVerticalAction_, &QAction::triggered,
          this, &MainWindow::flipVertical);

  imageMenu->addSeparator();

  grayscaleAction_ = imageMenu->addAction("&Grayscale");
  grayscaleAction_->setStatusTip("Convert image to grayscale");
  connect(grayscaleAction_,
          &QAction::triggered,
          this,
          &MainWindow::convertToGrayscale);
}

void MainWindow::createStatusBar()
{
  updateStatusBar();
}

void MainWindow::updateStatusBar()
{
  const QSize imageSize = viewer_->imageSize();
  const int zoomPercent =
      static_cast<int>(viewer_->zoomFactor() * 100.0);

  statusBar()->showMessage(
      QString("Image: %1 × %2    Zoom: %3%")
          .arg(imageSize.width())
          .arg(imageSize.height())
          .arg(zoomPercent));
}

void MainWindow::zoomIn()
{
  viewer_->zoomIn();
  updateStatusBar();
}

void MainWindow::zoomOut()
{
  viewer_->zoomOut();
  updateStatusBar();
}

void MainWindow::addRecentFile(const QString& fileName)
{
  recentFiles_.removeAll(fileName);
  recentFiles_.prepend(fileName);

  while (recentFiles_.size() > kMaxRecentFiles)
    recentFiles_.removeLast();

  updateRecentFilesMenu();
}

void MainWindow::updateRecentFilesMenu()
{
  recentMenu_->clear();

  for (const QString& fileName : recentFiles_)
  {
    QAction *action = recentMenu_->addAction(fileName);
    action->setData(fileName);

    connect(action, &QAction::triggered,
            this, &MainWindow::openRecentFile);
  }

  if (!recentFiles_.isEmpty())
  {
    recentMenu_->addSeparator();

    QAction *clearAction =
        recentMenu_->addAction("Clear Recent Files");

    connect(clearAction, &QAction::triggered,
            this, &MainWindow::clearRecentFiles);
  }

  recentMenu_->setEnabled(!recentFiles_.isEmpty());
}

void MainWindow::openRecentFile()
{
  QAction *action = qobject_cast<QAction*>(sender());
  if (!action)
    return;
  const QString fileName = action->data().toString();
  openImage(fileName);
}

void MainWindow::loadSettings()
{
  QSettings settings;
  recentFiles_ = settings.value("recentFiles").toStringList();
  updateRecentFilesMenu();
}

void MainWindow::saveSettings()
{
  QSettings settings;
  settings.setValue("recentFiles", recentFiles_);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  saveSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::clearRecentFiles()
{
  recentFiles_.clear();
  updateRecentFilesMenu();
  saveSettings();
}

void MainWindow::createToolBar()
{
  QToolBar *toolBar = addToolBar("Main Toolbar");

  toolBar->addAction(openAction_);
  toolBar->addSeparator();

  toolBar->addAction(zoomInAction_);
  toolBar->addAction(zoomOutAction_);
  toolBar->addSeparator();

  toolBar->addAction(fitAction_);
  toolBar->addAction(actualSizeAction_);

  toolBar->addSeparator();
  toolBar->addAction(rotateLeftAction_);
  toolBar->addAction(rotateRightAction_);

  toolBar->addSeparator();

  toolBar->addAction(flipHorizontalAction_);
  toolBar->addAction(flipVerticalAction_);

  toolBar->addSeparator();
  toolBar->addAction(grayscaleAction_);
}

void MainWindow::rotateLeft()
{
  viewer_->rotateLeft();
  updateStatusBar();
}

void MainWindow::rotateRight()
{
  viewer_->rotateRight();
  updateStatusBar();
}

void MainWindow::flipHorizontal()
{
  viewer_->flipHorizontal();
  updateStatusBar();
}

void MainWindow::flipVertical()
{
  viewer_->flipVertical();
  updateStatusBar();
}

void MainWindow::convertToGrayscale()
{
  ImageProcessor processor;

  const QImage grayscaleImage = processor.toGrayscale(viewer_->image());

  if (grayscaleImage.isNull())
    return;

  viewer_->setImage(grayscaleImage);
  updateStatusBar();
}
