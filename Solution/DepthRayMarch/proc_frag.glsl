uniform vec2 viewport;
uniform sampler2D colorMap;
uniform sampler2D depthMap;

void main(){
	vec2 uv = gl_FragCoord.xy/viewport;
	uv.y = 1- uv.y;
	
	gl_FragColor = vec4(texture2D(colorMap,uv).xyz,1);

} 