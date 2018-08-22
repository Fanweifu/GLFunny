in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D baseTex;

void main(){
    vec4 matcolor;
	matcolor += gl_FrontMaterial.emission;
    matcolor += (gl_LightSource[0].ambient*+gl_LightModel.ambient)*gl_FrontMaterial.ambient;
    
    vec4 lightPos = gl_LightSource[0].position;
    vec3 lightori =  normalize(lightPos.xyz);
    

    vec3 halfv = normalize(gl_LightSource[0].halfVector.xyz);
    vec3 normalz = normalize(Normal);

    if (!gl_FrontFacing) {
        normalz = -normalz;
    }
	float cosVal = dot(normalz, lightori);
    vec4 diffuse = gl_LightSource[0].diffuse* gl_FrontMaterial.diffuse* max(cosVal,0);
    vec4 specular = gl_LightSource[0].specular* gl_FrontMaterial.specular * pow(max(dot(normalz,halfv),0), gl_FrontMaterial.shininess);


    matcolor += (diffuse +specular);
    gl_FragColor = matcolor*texture2D(baseTex, TexCoord);

		 
}