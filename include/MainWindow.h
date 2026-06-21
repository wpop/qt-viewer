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

private:
  void createViewer();
  void createMenus();
  void createStatusBar();
  void updateStatusBar();

private slots:
  void openImage();
  void openImage(const QString& fileName);
  void fitToWindow();
  void actualSize();
  void zoomIn();
  void zoomOut();
};
