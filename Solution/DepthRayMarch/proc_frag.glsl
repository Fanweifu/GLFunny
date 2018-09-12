uniform vec2 viewport;
uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform sampler2D stencilMap;
uniform mat4 prjMatInv;

float zdepth(vec2 uv){
	float depth = texture2D(depthMap,uv).r;
	vec4 dv = vec4(uv,depth,1)*2-1;
	vec4 pos = prjMatInv*dv;
	pos/=pos.w;
	return pos.z;
}

float stencil(vec2 uv){
	return texture2D(depthMap,uv).g;
}

void main(){
	vec2 uv = gl_FragCoord.xy/viewport;
	
	float k1 = pow(dFdx(zdepth(uv)),2)+pow(dFdy(zdepth(uv)),2);
	float k2 = pow(dFdx(stencil(uv)),2)+pow(dFdx(stencil(uv)),2);

	float stval = stencil(uv);
	gl_FragColor = vec4(vec3(k1+stval*0.2),1);

} 