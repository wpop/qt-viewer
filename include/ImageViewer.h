#pragma once

#include <QGraphicsView>

class ImageViewer : public QGraphicsView
{
  Q_OBJECT

public:
  explicit ImageViewer(QWidget *parent = nullptr);
};
