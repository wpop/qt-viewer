#include "ImageViewer.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>

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
  resetTransform();
  fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);

  if (scene() && !scene()->items().isEmpty())
  {
    resetTransform();
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
  }
}
