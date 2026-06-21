#include "ImageViewer.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent)
{
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::ScrollHandDrag);
  auto *graphicsScene = new QGraphicsScene(this);
  setScene(graphicsScene);
  pixmapItem_ = graphicsScene->addPixmap(QPixmap());
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void ImageViewer::setImage(const QImage& image)
{
  pixmapItem_->setPixmap(QPixmap::fromImage(image));
  scene()->setSceneRect(pixmapItem_->boundingRect());
  fitToWindow();
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
  constexpr double kZoomFactor = 1.25;

  const double factor = (event->angleDelta().y() > 0)
                            ? kZoomFactor
                            : 1.0 / kZoomFactor;

  scale(factor, factor);
}

void ImageViewer::fitToWindow()
{
  if (!scene() || scene()->items().isEmpty())
    return;

  resetTransform();
  fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}
