#include "layer.h"

Layer::Layer()
{
}
void Layer::ondraw() {
    for (auto item : items) {
        item->draw();
    }
}

void Layer::addUnique(Shape *shp) {
    add(shp);
}

Shape* Layer::removeAt(int idx) {
    return items.at(idx);
}

bool Layer::remove(Shape* shp) {
    return true;
}

