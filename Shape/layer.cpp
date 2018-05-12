#include "layer.h"

Layer::Layer()
{
}
void Layer::ondraw() {
    for (auto item : items) {
        Shape *shppt = dynamic_cast<Shape *>(item);
        shppt->draw();
    }
}

void Layer::addUnique(Shape *shp) {
    add(shp);
}

Shape* Layer::removeAt(int idx) {
    return NULL;
}

bool Layer::remove(Shape *shp) {
    return true;
}