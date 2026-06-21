#pragma once

#include <QGraphicsView>
#include <QImage>
#include <QSize>
#include <QString>

class QGraphicsPixmapItem;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class ImageViewer : public QGraphicsView
{
  Q_OBJECT

public:
  explicit ImageViewer(QWidget *parent = nullptr);
  void setImage(const QImage& image);
  void fitToWindow();
  void actualSize();
  QSize imageSize() const;
  double zoomFactor() const;
  void zoomIn();
  void zoomOut();

protected:
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

signals:
  void imageDropped(const QString& fileName);

private:
  QGraphicsPixmapItem *pixmapItem_;
  bool fitMode_ = true;
};
