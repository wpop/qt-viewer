#pragma once

#include <QMainWindow>

class ImageViewer;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

private:
  ImageViewer *viewer_;

private slots:
  void openImage();
};
