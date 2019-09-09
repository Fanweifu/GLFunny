#ifndef VAOARRAY_H
#define VAOARRAY_H


#include<assert.h>
#include<map>
#include<vector>
#include"gldef.h"
#include"object.h"


class BindData : public glObject {
public:
    BindData();
    ~BindData();

    void init() override;
    void unInit() override;
    void bind() override;
    void unBind() override;
    
    virtual void renderData(GLenum mode,int cnt) = 0;

protected:
    GLuint mVBO = 0;
    void initBuffs();

};

//adapt to float data
struct dataLayout
{
	const char* attriName;
	int layoutLoc;
	int stride;
	int elementsize;
	const void* ptrshift;
};

class AttribVAO :public BindData
{
public:

    AttribVAO() : BindData() {}
   
	void updateBuffer(const void* pData, int bytes);
	//void updateIndex(const unsigned short* idxs);
	void setLayouts(const std::vector<dataLayout>& layouts);
    void renderData(GLenum mode,int cnt) override;
protected:
	std::vector<dataLayout> m_layouts;
	
private:
};







#endif // VAOARRAY_H
