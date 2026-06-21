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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Qt Viewer");
  resize(1000, 700);

  viewer_ = new ImageViewer(this);
  setCentralWidget(viewer_);

  QMenu *fileMenu = menuBar()->addMenu("&File");
  QAction *openAction = fileMenu->addAction("&Open Image...");

  connect(openAction, &QAction::triggered,
          this, &MainWindow::openImage);

  QMenu *viewMenu = menuBar()->addMenu("&View");

  QAction *fitAction = viewMenu->addAction("Fit to &Window");
  connect(fitAction, &QAction::triggered,
          this, &MainWindow::fitToWindow);

  QAction *actualSizeAction = viewMenu->addAction("&Actual Size");
  connect(actualSizeAction, &QAction::triggered,
          this, &MainWindow::actualSize);

  statusBar()->showMessage("Ready");
}

void MainWindow::openImage()
{
  QString fileName = QFileDialog::getOpenFileName(
      this,
      "Open Image",
      QString(),
      "Images (*.png *.jpg *.jpeg *.bmp)"
  );

  if (fileName.isEmpty())
    return;

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
}

void MainWindow::fitToWindow()
{
  viewer_->fitToWindow();
}

void MainWindow::actualSize()
{
  viewer_->actualSize();
}
