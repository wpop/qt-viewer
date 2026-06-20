#include "MainWindow.h"
#include "ImageViewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Qt Viewer");
  resize(1000, 700);

  auto *viewer = new ImageViewer(this);
  setCentralWidget(viewer);
}
