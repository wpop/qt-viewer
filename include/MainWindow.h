#pragma once

#include <QMainWindow>
#include <QStringList>

class QMenu;
class ImageViewer;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

private:
  ImageViewer *viewer_;
  QMenu *recentMenu_ = nullptr;
  QStringList recentFiles_;

private:
  void createViewer();
  void createMenus();
  void createStatusBar();
  void updateStatusBar();
  void addRecentFile(const QString& fileName);
  void updateRecentFilesMenu();

private slots:
  void openImage();
  void openImage(const QString& fileName);
  void fitToWindow();
  void actualSize();
  void zoomIn();
  void zoomOut();
  void openRecentFile();
};
