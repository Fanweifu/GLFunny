uniform float     iTime;
uniform vec2      viewport;
uniform mat4      prjInvMat;
uniform mat4      mdlInvMat;

vec3 yztozy(vec3 p){
    return vec3(p.x,p.z,p.y);
}

vec2 coordToUV(vec2 coord,vec2 size){
    return 2.0*coord/size-1.0;
}

void setupCamera(vec2 uv,out vec3 campos, out vec3 dir){
     vec4 camdir = vec4(uv,1,1)*prjInvMat;
     camdir = camdir/camdir.w;

     vec4 camv = vec4(0,0,0,1);
     camv = camv*mdlInvMat;
     campos = camv.xyz;

     vec4 dirp = camdir*mdlInvMat;
     dir = normalize((dirp-camv).xyz);
}


const float coeiff = 0.25;
const vec3 totalSkyLight = vec3(0.3, 0.5, 1.0);

vec3 mie(float dist, vec3 sunL){
    return max(exp(-pow(dist, 0.25)) * sunL - 0.4, 0.0);
}

vec3 getSky(vec3 dir, vec3 sunPos){
	
	float sunDistance = distance(dir , clamp(sunPos, -1.0, 1.0));
	
	float scatterMult = clamp(sunDistance, 0.0, 1.0);
	float sun = clamp(1.0 - smoothstep(0.01, 0.011, scatterMult), 0.0, 1.0);
	
	float dist = dir.z;
	dist = (coeiff * mix(scatterMult, 1.0, dist)) / dist;
    
    vec3 mieScatter = mie(sunDistance, vec3(1.0));
	
	vec3 color = dist * totalSkyLight;
    
    color = max(color, 0.0);
    
	color = max(mix(pow(color, 1.0 - color),
	color / (2.0 * color + 0.5 - color),
	clamp(sunPos.z * 2.0, 0.0, 1.0)),0.0)
	+ sun + mieScatter;
	
	color *=  (pow(1.0 - scatterMult, 10.0) * 10.0) + 1.0;
	
	float underscatter = distance(sunPos.z * 0.5 + 0.5, 1.0);
	
	color = mix(color, vec3(0.0), clamp(underscatter, 0.0, 1.0));
	
	return color;	
}

void main(){
    
    vec2 uv = coordToUV(gl_FragCoord.xy,viewport);
    vec3 pos,dir;
    setupCamera(uv,pos,dir);


    vec3 sunPos =  gl_LightSource[0].position.xyz;
    
    

	vec3 color = getSky(dir,sunPos);
	
	color = color / (2.0 * color + 0.5 - color);
	
	gl_FragColor = vec4(dir, 1.0);
}