layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in vec3 aNormal;
out vec2 TexCoord;
out vec3 Normal;

void main(){
	Normal = gl_NormalMatrix*aNormal;
	TexCoord = aTexCoord;
    gl_Position = gl_ModelViewProjectionMatrix*aPos;
		 
}