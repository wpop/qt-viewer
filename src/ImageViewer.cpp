#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent)
{
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::ScrollHandDrag);
}
