#ifndef LAYER_H
#define LAYER_H

#include"shape.h"
#include<assert.h>
#include<vector>

class Layer : public Shape
{
public:
    Layer();

    virtual void add(Shape* shp) {  items.push_back(shp); }
    virtual void addUnique(Shape *shp);
    virtual Shape* removeAt(int idx);
    virtual bool remove(Shape *shp);
    virtual Shape*  getItemAt(unsigned int idx) { assert(idx >= 0 && idx < count()); return items.at(idx); }
    virtual unsigned int count() { return items.size(); }

protected:

    std::vector<Shape*> items;

    void ondraw();
};

#endif // LAYER_H
