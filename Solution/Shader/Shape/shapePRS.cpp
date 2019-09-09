#include "shapePRS.h"
#include<cstring>

ShapePRS::ShapePRS()
{
}

void ShapePRS::draw() {
	if (!visible) return;

	glPushMatrix();

	glMultMatrixf(glm::value_ptr(modelmat));

	if (texture0 > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
	}

	if (texture1 > 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}

	if (texture2 > 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture2);
	}

	ondraw();

	ShapeBase::draw();

	glPopMatrix();
}

void ShapePRS::updateModel() {
	modelmat = glm::translate(glm::mat4(1.0), pvec);
	modelmat = glm::scale(modelmat, svec);
	modelmat = glm::rotate(modelmat, rvec.z*DEG2RAD, glm::vec3(0, 0, 1));
	modelmat = glm::rotate(modelmat, rvec.y*DEG2RAD, glm::vec3(0, 1, 0));
	modelmat = glm::rotate(modelmat, rvec.x*DEG2RAD, glm::vec3(1, 0, 0));

	modelmatInv = glm::inverse(modelmat);
}

void ShapePRS::toLocalPos(float &x, float &y, float &z) {
	glm::vec4 vl(x, y, z, 1);
	glm::vec4 wv = vl*modelmatInv;

	x = wv[0];
	y = wv[1];
	z = wv[2];
}

void ShapePRS::toWorldPos(float &x, float &y, float &z) {
	glm::vec4 vl(x, y, z, 1);
	glm::vec4 wv = vl*modelmat;

	x = wv[0];
	y = wv[1];
	z = wv[2];
}

void ShapePRS::toLocalDir(float & x, float & y, float & z)
{
	toLocalPos(x, y, z);
	float ox, oy, oz;
	toLocalPos(ox, oy, oz);

	x -= ox; y -= oy; z -= oz;
}

void ShapePRS::toWorldDir(float & x, float & y, float & z)
{
	toWorldPos(x, y, z);
	float ox, oy, oz;
	toWorldPos(ox, oy, oz);

	x -= ox; y -= oy; z -= oz;
}