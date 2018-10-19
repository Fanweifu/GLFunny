#version 120

uniform sampler2D sdfTex;
uniform vec2 texSize =vec2(32,32);//滚轮切换不同分辨率的SDF纹理
uniform vec4 backColor = vec4(1,1,1,0);
uniform vec4 lineColor = vec4(1,1,1,1);
uniform vec4 fillColor = vec4(0,1,1,1);
uniform float lineDist = 0.05;// 描边距离
uniform float fillDist = 0.005;//填充距离 方向左右调整
uniform float smoothDelta = 0.001;//描边距离 方向上下调整

varying vec2 texcoord;


float sdfFunc(vec2 coord){
	return texture2D(sdfTex,coord).r-0.5;
}

//float sdfCircle(vec2 coord, vec2 center, float radius)
//{
//	return distance(coord,center) - radius;
//}

//float sdfBilinear(vec2 coord){
//	vec2 pixcoord = coord*texSize;
//	float x = pixcoord.x,y = pixcoord.y;
//	float x1 = floor(x),x2 =ceil(x),y1 = floor(y),y2 = ceil(y);
//	return sdfFunc(vec2(x1,y1)/texSize)*(x2-x)*(y2-y)+sdfFunc(vec2(x2,y1)/texSize)*(x-x1)*(y2-y)+sdfFunc(vec2(x1,y2)/texSize)*(x2-x)*(y-y1)+sdfFunc(vec2(x2,y2)/texSize)*(x-x1)*(y-y1);
//}


vec4 render(float d) 
{
	float anti = max(smoothDelta,0);
	vec4 color = backColor;
	color = mix(lineColor,color,smoothstep(-anti,anti,d-lineDist-fillDist));
	color = mix(fillColor,color,smoothstep(-anti,anti,d-fillDist));

	return color;
}



void main() 
{
	float d = sdfFunc(texcoord);
	
	gl_FragColor = render(d);
}