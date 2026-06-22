#pragma once

#include <QMainWindow>
#include <QStringList>

class QMenu;
class ImageViewer;
class QCloseEvent;
class QAction;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:
  void openImage();
  void openImage(const QString& fileName);
  void saveImageAs();
  void fitToWindow();
  void actualSize();
  void zoomIn();
  void zoomOut();
  void rotateLeft();
  void rotateRight();
  void flipHorizontal();
  void flipVertical();
  void convertToGrayscale();
  void openRecentFile();
  void clearRecentFiles();

private:
  // UI
  void createViewer();
  void createMenus();
  void createFileMenu();
  void createViewMenu();
  void createImageMenu();
  void createStatusBar();
  void updateStatusBar();
  void createToolBar();

  QAction *rotateLeftAction_ = nullptr;
  QAction *rotateRightAction_ = nullptr;

  // Recent files
  void addRecentFile(const QString& fileName);
  void updateRecentFilesMenu();

  // Settings
  void loadSettings();
  void saveSettings();

private:
  ImageViewer *viewer_ = nullptr;
  QMenu *recentMenu_ = nullptr;
  QStringList recentFiles_ {};

  QAction *openAction_ = nullptr;
  QAction *saveAsAction_ = nullptr;
  QAction *zoomInAction_ = nullptr;
  QAction *zoomOutAction_ = nullptr;
  QAction *fitAction_ = nullptr;
  QAction *actualSizeAction_ = nullptr;

  QAction *flipHorizontalAction_ = nullptr;
  QAction *flipVerticalAction_ = nullptr;

  QAction *grayscaleAction_ = nullptr;
};
