#include "MainWindow.h"
#include "ImageViewer.h"
#include "ImageLoader.h"
#include <QImage>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Qt Viewer");
  resize(1000, 700);

  createViewer();
  createMenus();
  createStatusBar();
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
}

void MainWindow::actualSize()
{
  viewer_->actualSize();
}

void MainWindow::createViewer()
{
  viewer_ = new ImageViewer(this);
  setCentralWidget(viewer_);

  // Disambiguate the overloaded openImage() slot.
  connect(viewer_,
          &ImageViewer::imageDropped,
          this,
          static_cast<void (MainWindow::*)(const QString&)>(
              &MainWindow::openImage));
}

void MainWindow::createMenus()
{
  // File menu actions
  QMenu *fileMenu = menuBar()->addMenu("&File");
  QAction *openAction = fileMenu->addAction("&Open Image...");
  openAction->setShortcut(QKeySequence::Open);

  recentMenu_ = fileMenu->addMenu("Open &Recent");
  fileMenu->addSeparator();

  // Explicitly select the correct overloaded slot for connect().
  connect(openAction,
          &QAction::triggered,
          this,
          static_cast<void (MainWindow::*)()>(&MainWindow::openImage));

  // View menu actions
  QMenu *viewMenu = menuBar()->addMenu("&View");

  QAction *zoomInAction = viewMenu->addAction("Zoom &In");
  zoomInAction->setShortcut(QKeySequence::ZoomIn);
  connect(zoomInAction, &QAction::triggered,
          this, &MainWindow::zoomIn);

  QAction *zoomOutAction = viewMenu->addAction("Zoom &Out");
  zoomOutAction->setShortcut(QKeySequence::ZoomOut);
  connect(zoomOutAction, &QAction::triggered,
          this, &MainWindow::zoomOut);

  viewMenu->addSeparator();

  QAction *fitAction = viewMenu->addAction("Fit to &Window");
  fitAction->setShortcut(QKeySequence("Ctrl+F"));
  connect(fitAction, &QAction::triggered,
          this, &MainWindow::fitToWindow);

  QAction *actualSizeAction = viewMenu->addAction("&Actual Size");
  actualSizeAction->setShortcut(QKeySequence("Ctrl+0"));
  connect(actualSizeAction, &QAction::triggered,
          this, &MainWindow::actualSize);
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

  constexpr int kMaxRecentFiles = 5;

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
