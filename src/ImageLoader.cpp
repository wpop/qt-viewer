#include "ImageLoader.h"

#include <QImage>

QImage ImageLoader::load(const QString& fileName) const
{
  return QImage(fileName);
}
