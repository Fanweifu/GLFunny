#version 130

uniform vec2 viewport;
uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform usampler2D stencilMap;
uniform int smoothl = 2;

void main(){
	vec2 uv = gl_FragCoord.xy/viewport;
	vec4 color;
	vec2 offset = (vec2(smoothl-1)*-0.5)/viewport;
	for(float i=0;i<smoothl;i++)
		for(float j=0;j<smoothl;j++)
			color+=texture2D(colorMap,uv+offset);
	color/=(smoothl*smoothl);
	
	
    
	//gl_FragColor = vec4(vec3(stval*0.5f),1);
	gl_FragColor =  color;
	
} 