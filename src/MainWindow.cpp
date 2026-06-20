#include "MainWindow.h"
#include "ImageViewer.h"
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Qt Viewer");
  resize(1000, 700);

  auto *viewer = new ImageViewer(this);
  setCentralWidget(viewer);

  QMenu *fileMenu = menuBar()->addMenu("&File");

  QAction *openAction = fileMenu->addAction("&Open Image...");

  connect(openAction, &QAction::triggered,
          this, &MainWindow::openImage);
}

void MainWindow::openImage()
{
  QMessageBox::information(
      this,
      "Qt Viewer",
      "Open Image will be implemented in the next step."
  );
}
