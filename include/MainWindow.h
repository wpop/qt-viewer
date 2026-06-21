#pragma once

#include <QMainWindow>
#include <QStringList>

class QMenu;
class ImageViewer;
class QCloseEvent;

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

private:
  // UI
  void createViewer();
  void createMenus();
  void createStatusBar();
  void updateStatusBar();

  // Recent files
  void addRecentFile(const QString& fileName);
  void updateRecentFilesMenu();

  // Settings
  void loadSettings();
  void saveSettings();

private:
  ImageViewer *viewer_ = nullptr;
  QMenu *recentMenu_ = nullptr;
  QStringList recentFiles_;
};
