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
  void fitToWindow();
  void actualSize();
  void zoomIn();
  void zoomOut();
  void openRecentFile();
  void clearRecentFiles();
  void rotateLeft();
  void rotateRight();
  void flipHorizontal();
  void flipVertical();

private:
  // UI
  void createViewer();
  void createMenus();
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
  QAction *zoomInAction_ = nullptr;
  QAction *zoomOutAction_ = nullptr;
  QAction *fitAction_ = nullptr;
  QAction *actualSizeAction_ = nullptr;

  QAction *flipHorizontalAction_ = nullptr;
  QAction *flipVerticalAction_ = nullptr;

};
