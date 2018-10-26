#ifndef LAYER_H
#define LAYER_H

#include"shapePRS.h"
#include<assert.h>
#include<vector>

class Layer : public ShapePRS
{
public:
    Layer();

    virtual void add(ShapeBase* shp) {  items.push_back(shp); }
    virtual void addUnique(ShapeBase *shp);
    virtual ShapeBase* removeAt(int idx);
    virtual bool remove(ShapeBase *shp);
    virtual ShapeBase*  getItemAt(unsigned int idx) { assert(idx >= 0 && idx < count()); return items.at(idx); }
    virtual unsigned int count() { return items.size(); }

protected:

    std::vector<ShapeBase*> items;

    void ondraw();
};

#endif // LAYER_H
