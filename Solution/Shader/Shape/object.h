#pragma once

class glObject
{
public:
    glObject() {};
    ~glObject() {};
protected:
    bool isVaild = false;
    bool isInited = false;
    bool isBinded = false;

    virtual void init()=0;
    virtual void unInit()=0;
    virtual void bind()=0;
    virtual void unBind()=0;
    virtual bool vaild() { return isVaild; }
private:

};
