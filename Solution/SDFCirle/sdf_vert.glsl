//#version 120
varying vec2 texCoord;
varying vec3 normal;
varying vec3 position;
void main(){
	texCoord = gl_MultiTexCoord0.xy;
	normal = normalize(gl_NormalMatrix * gl_Normal);  
	position = vec3(gl_ModelViewMatrix*gl_Vertex);

	gl_Position  = gl_ProjectionMatrix*vec4(position,1);

}