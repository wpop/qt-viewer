#pragma once

#include <QGraphicsView>
#include <QImage>

class QGraphicsPixmapItem;

class ImageViewer : public QGraphicsView
{
  Q_OBJECT

public:
  explicit ImageViewer(QWidget *parent = nullptr);
  void setImage(const QImage& image);

protected:
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  QGraphicsPixmapItem *pixmapItem_;
};
