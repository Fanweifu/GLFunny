#version 120

//uniform vec2 tscl;

varying vec2 texcoord;


void main(){
	texcoord = gl_MultiTexCoord0.xy;
	
	gl_Position =   ftransform(); // vec4(coord,0,1);
	
}