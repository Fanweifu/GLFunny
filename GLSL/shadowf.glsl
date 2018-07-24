#version 130

in vec4 fragLight;
in vec2 texcoord;
in vec3 normal;

uniform bool enablePbr;
uniform int smoothLevel;
uniform float biasFactor;
uniform sampler2D baseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
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
    vec3 normalz = enablePbr?normalize(gl_NormalMatrix*texture2D(normalTex, texcoord).rgb):normalize(normal);

    if (!gl_FrontFacing) {
        normalz = -normalz;
    }
	float cosVal = dot(normalz, lightori);
	float tanval = sqrt(1-cosVal*cosVal)/abs(cosVal);

	
    vec4 diffuse = gl_LightSource[0].diffuse* gl_FrontMaterial.diffuse* max(cosVal,0);
    vec4 materialSpecular = enablePbr ? texture2D(specularTex, texcoord) : gl_FrontMaterial.specular;
    vec4 specular = gl_LightSource[0].specular* materialSpecular * pow( max(dot(normalz,halfv),0), gl_FrontMaterial.shininess);


    float shadowK = calcShadow(fragLight,tanval);
	
    matcolor += (1-shadowK)*(diffuse +specular);


    gl_FragColor = vec4(matcolor.xyz*(enablePbr?0.4:1),1) *texture2D(baseTex, texcoord);
    
}