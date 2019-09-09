

uniform sampler2D tex; 
uniform bool dev;
uniform float ratio;
in vec3 normal;
in vec4 color;
in vec2 texCoord;

void main(){	
	if(dev) gl_FragColor = vec4(fwidth(normal)*540,1);
	else{

		float r = ratio;
		r -= float(floor(r));
		gl_FragColor = mix(texture2D(tex,texCoord+normal.xy*r),texture2D(tex,texCoord+normal.xy*(r-1.0)),r);
	}
}