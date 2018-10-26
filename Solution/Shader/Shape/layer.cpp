#include "layer.h"

Layer::Layer()
{
}
void Layer::ondraw() {
    for (auto item : items) {
        item->draw();
    }
}

void Layer::addUnique(ShapeBase *shp) {
    add(shp);
}

ShapeBase* Layer::removeAt(int idx) {
    return items.at(idx);
}

bool Layer::remove(ShapeBase* shp) {
    return true;
}

