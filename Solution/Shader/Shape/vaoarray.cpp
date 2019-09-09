#include "vaoarray.h"
#include "stdio.h"

#define STATICDRAW(b) (b?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)

//void AttribVAO::init(int ptsNum,int buffs)
//{
//    vertexNum = ptsNum;
//    BindData::init(4);
//}
//
//void AttribVAO::setVertex3f(GLfloat * arr)
//{
//    CHECKBIND({
//    glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[VERTEX_VBO_LAYOUT]);
//    glBufferData(GL_ARRAY_BUFFER, vertexNum* 3 * sizeof(float), arr, STATICDRAW(true));
//    glVertexAttribPointer(VERTEX_VBO_LAYOUT, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
//    glEnableVertexAttribArray(VERTEX_VBO_LAYOUT);
//    })
//}
//
//void AttribVAO::setColor4f(GLfloat * arr)
//{
//    CHECKBIND({
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[COLOR_VBO_LAYOUT]);
//        glBufferData(GL_ARRAY_BUFFER, vertexNum * 4 * sizeof(float), arr, STATICDRAW(true));
//        glVertexAttribPointer(COLOR_VBO_LAYOUT, 4, GL_FLOAT, GL_TRUE, 0, (void*)0);
//        glEnableVertexAttribArray(COLOR_VBO_LAYOUT);
//    })
//}
//
//void AttribVAO::setNormal3f(GLfloat * arr)
//{
//    CHECKBIND({
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[NORMAL_VBO_LAYOUT]);
//        glBufferData(GL_ARRAY_BUFFER, vertexNum * NORMAL_SIZE * sizeof(float), arr, STATICDRAW(true));
//        glVertexAttribPointer(NORMAL_VBO_LAYOUT, NORMAL_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
//        glEnableVertexAttribArray(NORMAL_VBO_LAYOUT);
//    })
//}
//
//void AttribVAO::setTexCoord2f(GLfloat * arr)
//{
//    CHECKBIND({
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[TEXCOORD_VBO_LAYOUT]);
//        glBufferData(GL_ARRAY_BUFFER, vertexNum * TEXCOORD_SIZE * sizeof(float), arr, STATICDRAW(true));
//        glVertexAttribPointer(TEXCOORD_VBO_LAYOUT, TEXCOORD_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
//        glEnableVertexAttribArray(TEXCOORD_VBO_LAYOUT);
//    })
//}
//



void AttribVAO::updateBuffer(const void * pData, int bytes)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, bytes, pData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AttribVAO::setLayouts(const std::vector<dataLayout>& layouts)
{
	m_layouts = layouts;
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	for (dataLayout& ly : m_layouts) {
		if(ly.layoutLoc<0) continue;
		glVertexAttribPointer(ly.layoutLoc, ly.elementsize, GL_FLOAT,GL_FALSE,ly.stride,ly.ptrshift);
		glEnableVertexAttribArray(ly.layoutLoc);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AttribVAO::renderData(GLenum mode,int cnt)
{
    glBindVertexArray(m_handle);
    glDrawArrays(mode, 0, cnt);
    glBindVertexArray(0);
}

BindData::BindData()
{
}

BindData::~BindData()
{
    unInit();
}


void BindData::unInit() {
    if (!isInited) return;
    glDeleteVertexArrays(1, &m_handle);
    glDeleteBuffers(1, &mVBO);
    isInited = isVaild = isBinded = false;
}

void BindData::bind()
{
    glBindVertexArray(m_handle);
    isBinded = true;
}

void BindData::unBind()
{
    glBindVertexArray(0);
    isBinded = false;
}

void BindData::init()
{
    if (isInited) return;
    initBuffs();
    isInited = true;
}

void BindData::initBuffs()
{
    glGenVertexArrays(1, &m_handle);
    glGenBuffers(1, &mVBO);
}

