#include "delaunator.h"
#include "pixaloop.h"
#include "inpaint.h"
#include <assert.h>
bool pixaloop::loadSourceImg(const char * path)
{
	//load image 
	int cols, rows, chns;
	uchar* pimg = stbi_load(path, &cols, &rows, &chns, 4);
	if (pimg == nullptr) return false;
	originMap.copy((const RgbaC*)pimg, cols, rows);
	stbi_image_free(pimg);
	return true;
}

bool pixaloop::prepare()
{
	int w = originMap.width(), h = originMap.height();
	originTex.setTexImg(originMap.head(), w, h);
	float wf = float(w) / 2, hf = float(h) / 2;


	meshShd.loadVertexFile("vert.glsl");
	meshShd.loadFragFile("pointfrag.glsl");
	meshShd.link();

	texShd.loadVertexFile("vert.glsl");
	texShd.loadFragFile("texfrag.glsl");
	texShd.link();

	flowShd.loadVertexFile("vert.glsl");
	flowShd.loadFragFile("flowImage.glsl");
	flowShd.link();

	Mesh::activeVAO = true;
	imageQuad.drawStyle = DrawType::Quads;
	imageQuad.addPoint(-wf, -hf, 0, 0, 1, 0, 0, 1);
	imageQuad.addPoint(wf, -hf, 0, 1, 1, 0, 0, 1);
	imageQuad.addPoint(wf, hf, 0, 1, 0, 0, 0, 1);
	imageQuad.addPoint(-wf, hf, 0, 0, 0, 0, 0, 1);


	meshAnchors.drawStyle = DrawType::Points;
	meshRayPoints.drawStyle = DrawType::Points;
	meshRayLine.drawStyle = DrawType::Lines;
	meshcurve.drawStyle = DrawType::Points;
	flowMesh.drawStyle = DrawType::Triangles;


	addInitAnchor();
	buildAnimator();
	updateMVP();
	return true;
}

void pixaloop::procKeyEvent(unsigned char key, Action act)
{
	switch (key)
	{

	case 'q':
		scale *= 1.1f;
		updateMVP();
		break;
	case 'e':
		scale *= 0.9f;
		updateMVP();
		break;
	case 'w':
		smooth *= 1.1f;
		buildAnimator(); 
		break;
	case 's':
		smooth *= 0.9f;
		smooth = std::max(0.1f, smooth);
		buildAnimator(); 
		break;
	case ' ':
		showPoints = !showPoints;
		break;
	case 'r':
		buildAnimator();
		break;
	case 'd':
		shdDev = !shdDev;
		break;
	case 'c':
		clearVertex();
		break;
	case 'l':
		testFunc();
		break;
	}
}

void pixaloop::procMouseClickEvent(Button b, Action act, int x, int y)
{
	if (b == Button::LEFT) {
		if (act == Action::DOWN) beginNewCurve();
		else {
			endCurve();
			buildAnimator();
		}
	}
	else if (b == Button::RIGHT) {
		glm::vec2 vertex = windows2Vertex(x, y);
		addAnchor(vertex.x,vertex.y);
		buildAnimator();
	}
}

void pixaloop::procMouseDragEvent(int x, int y)
{
	if (!inEditLine) return;
	if (glm::distance(glm::vec2(x, y), glm::vec2(dx, dy)) < 10) return;
	dx = x, dy = y;
	glm::vec2 vertex = windows2Vertex(x, y);
	addCurvePoint(vertex.x, vertex.y);
}

void pixaloop::render()
{
	time += 0.01f;
	


	

	flowShd.bind();
	flowShd.setUniformMat4("mvp", glm::value_ptr(mvp));
	flowShd.setUniform1f("ratio", time);
	flowShd.setUniform1i("dev", shdDev);
	{
		originTex.bind(0);
		flowShd.setUniform1i("tex", 0);
		flowMesh.draw();

		originTex.unBind();
	}
	flowShd.unBind();

	
	if (showPoints) {
		meshShd.bind();
		meshShd.setUniformMat4("mvp", glm::value_ptr(mvp));
		meshAnchors.draw();
		meshRayPoints.draw();
		meshRayLine.draw();
		meshcurve.draw();
		meshShd.unBind();
	}
}

void pixaloop::getWindowSize(int & x, int & y)
{
	x = roundf(originMap.width()*scale);
	y = roundf(originMap.height()*scale);
}

bool checkPair(glm::ivec3 v,int x,int y) {
	return (v.x == x || v.y == x | v.z == x) && (v.x == y || v.y == y | v.z == y);
}

bool pixaloop::triangulation()
{
	std::vector<delaunator::vertex2D> pointlist;

	for (auto& point : anchors) {
		pointlist.push_back({ point.x,point.y });
	}

	for (auto& rayitem : simplifyRays) {
		pointlist.push_back({ rayitem.x,rayitem.y });
	}

	delaunator::Delaunator del(pointlist);

	int triCnt = del.triangles.size() / 3;

	std::vector<glm::ivec3> tris;
	bool has01 = false, has12 = false, has23 = false, has30 = false;
	for (size_t i = 0; i < triCnt; i++)
	{
		glm::ivec3 iv(del.triangles[3 * i], del.triangles[3 * i + 1], del.triangles[3 * i + 2]);
		has01 |= checkPair(iv, 0, 1);
		has12 |= checkPair(iv, 2, 1);
		has23 |= checkPair(iv, 2, 3);
		has30 |= checkPair(iv, 0, 3);
	}

	

	if (!(has01&has12&has23&has30)) {
		return false;
	}

	idxs = del.triangles;
	return true;
}



void pixaloop::buildflowMesh()
{
	flowMesh.clear();
	int anchorCnt = anchors.size();
	for (size_t& idx : idxs) {
		if (idx >= anchorCnt) {
			const glm::vec4& r = simplifyRays[idx - anchorCnt];
			glm::vec2 dyCoord = vertex2Coord(glm::vec2(r.x,r.y)) - vertex2Coord(glm::vec2(r.z, r.w));

			meshVertex vertex = {};
			vertex.vertex = glm::vec3(r.x,r.y, 0);
			vertex.coord = vertex2Coord(glm::vec2(r.x, r.y));
			vertex.normal = glm::vec3(dyCoord, 0);
			flowMesh.addPoint(vertex);
		}
		else {
			const glm::vec2& anchor = anchors[idx];
			meshVertex vertex = {};
			vertex.vertex = glm::vec3(anchor, 0);
			vertex.coord = vertex2Coord(anchor);
			flowMesh.addPoint(vertex);
		}
	}
}

void pixaloop::clearVertex()
{
	anchors.clear();
	meshAnchors.clear();
	simplifyRays.clear();
	meshRayLine.clear();
	meshRayPoints.clear();
	
	curves.clear();
	meshcurve.clear();

	flowMesh.clear();
	idxs.clear();

	addInitAnchor();
	buildAnimator();
}

void pixaloop::addInitAnchor()
{
	float wf = float(originMap.width()) / 2, hf = float(originMap.height()) / 2;
	float p = std::fmax(wf, hf);

	addAnchor(-p, -p);
	addAnchor(p, -p);
	addAnchor(p, p);
	addAnchor(-p, p);
}

void pixaloop::beginNewCurve()
{
	if (inEditLine) return;
	std::vector<glm::vec2> newItem;
	curves.push_back(newItem);
	inEditLine = true;
}

void pixaloop::endCurve()
{
	if (!inEditLine) return;
	inEditLine = false;
}

void pixaloop::addAnchor(float x, float y)
{
	anchors.push_back(glm::vec2(x, y));
	meshAnchors.addPoint(x, y, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1);
}

void pixaloop::addCurvePoint(float x, float y)
{
	int lastIdx = curves.size() - 1;
	if (lastIdx < 0) return;
	vector<glm::vec2>& list = curves[lastIdx];
	if (list.size() > 0 && list[list.size() - 1] == glm::vec2(x, y)) return;

	list.push_back(glm::vec2(x, y));

	meshVertex v;
	v.vertex = glm::vec3(x, y, 1);
	v.color = glm::vec4(0, 1, 1, 1);

	meshcurve.addPoint(v);
}

void diluteFixedSteps(const std::vector<glm::vec2>& input, std::vector<glm::vec2>& output,float step) {
	auto tempInput = input;
	output.clear();
	if (tempInput.size() == 0) return;
	output.push_back(tempInput[0]);

	float distSum = 0.0f;
	for (int i = 1; i < tempInput.size(); i++) {
		float subdist = glm::distance(tempInput[i], tempInput[i - 1]);
		if (distSum<step && distSum + subdist>step) {
			float ratio = (step - distSum) / subdist;
			glm::vec2 endpoint = glm::mix(tempInput[i - 1], tempInput[i], ratio);
			output.push_back(endpoint);
			tempInput.insert(tempInput.begin() + i, endpoint);
			distSum = 0;
		}
		else {
			distSum += subdist;
			if (i == tempInput.size() - 1) 
				output.push_back(*tempInput.end());
		}
	}


}

void diluteStep(const std::vector<glm::vec2>& input, std::vector<glm::vec4>& output,float step) {

	if (input.size() == 0) return;
	float distSum = 0;
	for (int i = 1; i < input.size(); i++) {
		distSum += glm::distance(input[i], input[i - 1]);
	}

	float fstep = 1.5 *distSum / (input.size() - 1);
	step = std::min(fstep, step);

	auto pre = input[0];
	distSum = 0.0f;
	for (int i = 1; i < input.size(); i++) {
		auto cur = input[i];

		float pointdist = glm::distance(cur, pre);
		if (distSum<step && distSum + pointdist>step) {
			float ratio = (step - distSum) / pointdist;
			auto endpoint = glm::mix(pre, cur, ratio);
			output.push_back(glm::vec4(pre,endpoint));
			pre = endpoint;
			distSum = 0;
			i--;
			continue;
		}
		
		if (i == input.size() - 1 && pre == input[0]) {
			output.push_back(glm::vec4(pre, cur));
		}
	}

	

}

void diluteAngle(const std::vector<glm::vec2>& input, std::vector<glm::vec2>& output, float cosval) {

	if (input.size() <= 2) {
		output = input;
		return;
	}

	auto pre1 = input[0];
	auto pre2 = input[1];
	output.push_back(pre1);
	for (int i = 1; i < input.size()-1; i++) {
		auto cur = input[i];
		auto next = input[i + 1];
		auto v1 = pre2 - pre1, v2 = next - cur;
		float csVal = glm::dot(v1, v2) / glm::length(v1) / glm::length(v2);
		if (csVal < cosval) {
			output.push_back(glm::vec2(cur));
			pre1 = cur;
			pre2 = next;
			continue;
		}
	}
	output.push_back(input[input.size() - 1]);

}


#define  COS15DEG 0.985f
void pixaloop::simlifyCurve(float tdist)
{
	
	simplifyRays.clear();

	for (std::vector<glm::vec2>&list : curves) {
		std::vector<glm::vec2> outlist;
		diluteAngle(list, outlist, COS15DEG);
		diluteStep(outlist, simplifyRays, tdist);
	}

	meshRayPoints.clear();
	meshRayLine.clear();

	for (auto& ray : simplifyRays){
		meshVertex v;
		v.vertex = glm::vec3(ray.x,ray.y,1);
		v.color = glm::vec4(1, 1, 0, 1);
		meshRayPoints.addPoint(v);
		meshRayLine.addPoint(v);
		v.vertex = glm::vec3(ray.z,ray.w, 1);
		v.color = glm::vec4(1, 0, 1, 1);
		meshRayPoints.addPoint(v);
		meshRayLine.addPoint(v);
	}


}

void pixaloop::buildAnimator()
{
	float dt = getsplitDist();
	do {
		simlifyCurve(dt);
		dt *= 1.01;
	} while (!triangulation());
	
	buildflowMesh();
}

void pixaloop::updateMVP()
{
	model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1));
	view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	int w, h;
	getWindowSize(w, h);
	float wf = float(w) / 2, hf = float(h) / 2;
	prj = glm::ortho(-wf, wf, -hf, hf, 0.0f, 2.0f);
	mvp = prj*view*model;
}

void pixaloop::testFunc()
{
	//simlifyCurve();

	anchors.clear();
	addInitAnchor();

	for (const auto &item : simplifyRays) {
		addAnchor(item.x, item.y);
	}
	simplifyRays.clear();
	meshRayLine.clear();
	meshRayPoints.clear();

	triangulation();
	buildflowMesh();
}

float pixaloop::getsplitDist()
{
	return std::min(originMap.width(), originMap.height()) / 15.0f;
}

glm::vec2 pixaloop::vertex2Coord(glm::vec2 vertex)
{
	glm::vec2 imgcoord(vertex.x + float(originMap.width()) / 2, float(originMap.height()) / 2 - vertex.y);
	return imgcoord / glm::vec2(originMap.width(), originMap.height());
}

glm::vec2 pixaloop::windows2Vertex(int x, int y)
{
	int w, h;
	getWindowSize(w, h);
	return glm::vec2(x - float(w) / 2,  float(h) / 2 - y )/scale;
}
