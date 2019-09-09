#version 330 compatibility

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec4 aColor;

uniform mat4 mvp;

out vec2 texCoord;
out vec3 normal;
out vec4 color;
void main(){
	texCoord = aTexCoord;
	color = aColor;
	normal = aNormal;
	gl_Position =  mvp*vec4(aPos,1.0);
}