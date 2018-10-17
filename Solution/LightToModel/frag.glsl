#version 330 core

uniform vec3 lightDirInModel;
uniform vec3 camerPosInModel;
uniform sampler2D baseTex;

in vec2 texcoord;

vec4 lightModel(vec3 eyedir,vec3 normal,vec3 light){
	vec3 halfvec = normalize(-eyedir,light);
	return max(dot(normal,light),0)*0.5+pow(dot(normal,helfvec)
}

void main(){
	
}