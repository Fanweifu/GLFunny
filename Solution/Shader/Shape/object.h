#pragma once

#define CHECKBIND(proc)     if(isBinded) proc else{bind(); proc unBind();}

class glObject
{
public:
    glObject() {};
    ~glObject() {};
    unsigned int ObjectID() { return m_objID; }
protected:
    bool isVaild = false;
    bool isInited = false;
    bool isBinded = false;
    unsigned int m_objID = 0;

    virtual void init() = 0;
    virtual void unInit() = 0;
    virtual void bind() = 0;
    virtual void unBind() = 0;
    virtual bool vaild() { return isVaild; }
private:
};
