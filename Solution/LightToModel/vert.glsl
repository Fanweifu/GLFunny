#version 330 compatibility
 
layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aTexCoord;


out vec3 lightDirInModel;
out vec3 eyePosInModel;

void main(){
	gl_Position = gl_ModelViewProjectionMatrix*vec4(aPos,1);
	lightDirInModel = vec3(gl_ModelViewMatrixInverse*vec4(gl_LightSource[0].position));
	eyePosInModel = vec3(gl_ModelViewMatrixInverse*vec4(0,0,0,1));
}

