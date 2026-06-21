#pragma once

#include <QGraphicsView>
#include <QImage>
#include <QSize>

class QGraphicsPixmapItem;

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

protected:
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  QGraphicsPixmapItem *pixmapItem_;
  bool fitMode_ = true;
};
