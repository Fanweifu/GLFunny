#version 130

void main(){
	vec4 centre = gl_ModelViewMatrix*vec4(0,0,0,1);
	gl_Position = gl_ProjectionMatrix*vec4((gl_Vertex+centre).xyz,1);
}