#version 330 compatibility

in vec4 fragLight;
in vec2 texcoord;
in vec3 normal;

uniform int smoothLevel;
uniform float biasFactor;
uniform sampler2D baseTex;
uniform sampler2D depthTex;

float calcShadow(vec4 frag,float tanval){
	frag/=frag.w;
    frag = (frag+1.0)/2;
	float viewdepth = frag.z;

    if(frag.z > 1.0)
        return 0.0;
    float bias = biasFactor*tanval;
	
	float offset =  0;

	int cnt = (2*smoothLevel+1)*(2*smoothLevel+1);
	vec2 mapSize = textureSize(depthTex,0);
	
	for(int i=-smoothLevel;i<=smoothLevel;i++){
		for(int j=-smoothLevel;j<=smoothLevel;j++){
			offset+= viewdepth-bias>texture2D(depthTex, frag.xy+ vec2(i,j)/mapSize).r?1:0;
		}
	}

    offset/=cnt;
	
	if(frag.z>1.0) return 0;

	float disfactor = smoothstep(-0.5,-0.4,-distance(frag.xy,vec2(0.5,0.5)));

	return offset*disfactor;
}


void main(){
    
    vec4 matcolor;
	matcolor += gl_FrontMaterial.emission;
    matcolor += (gl_LightSource[0].ambient*+gl_LightModel.ambient)*gl_FrontMaterial.ambient;
    
    vec4 lightPos = gl_LightSource[0].position;
    vec3 lightori =  normalize(lightPos.xyz);
    

    vec3 halfv = normalize(gl_LightSource[0].halfVector.xyz);
    vec3 normalz = normalize(normal);

    
	float cosVal = dot(normalz, lightori);
	float tanval = sqrt(1-cosVal*cosVal)/abs(cosVal);
    float specK = pow(max(dot(normalz, halfv), 0.0), gl_FrontMaterial.shininess) ;
	
    vec4 diffuse = gl_LightSource[0].diffuse* gl_FrontMaterial.diffuse* max(cosVal,0);
    vec4 specular = gl_LightSource[0].specular* gl_FrontMaterial.specular * specK;


    float shadowK = calcShadow(fragLight,tanval);
    matcolor = vec4(0.5, 0.5, 0.5, 1);


    matcolor += /*(1-shadowK)**/(diffuse + specular);


    gl_FragColor = vec4(vec3(specK<0?-0.5:0.5),1)+ vec4(0.8, 0.8, 0.8, 1) /** texture2D(baseTex, texcoord)*/;
    
}