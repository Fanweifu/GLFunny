uniform sampler2D tex; 

in vec3 normal;
in vec4 color;
in vec2 texCoord;

void main(){
	gl_FragColor = texture2D(tex,texCoord);
}