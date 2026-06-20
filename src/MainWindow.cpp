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
