#version 130

in vec3 worldpos;
in vec2 texcoord;
in vec3 normal;

uniform sampler2D baseTex;
uniform sampler2D depthTex;
uniform mat4 lightspace;

void calcDepth(vec3 pos,out float viewdepth,out float mapdepth){
    vec4 viewpos = vec4(pos,1)*lightspace;
    //viewpos /= viewpos.w;
    viewdepth = viewpos.z;
    mapdepth = texture2D(depthTex,(viewpos.xy+1)/2).r;    
}

void main(){
    
    vec4 matcolor = gl_FrontMaterial.emission;
    matcolor += (gl_LightSource[0].ambient+gl_LightModel.ambient)*gl_FrontMaterial.ambient;
    
    vec4 lightPos = gl_LightSource[0].position;
    vec3 lightori;

    if(lightPos.w==0) lightori = normalize(lightPos.xyz);
    else lightori = normalize(lightPos.xyz/lightPos.w-worldpos);

    vec3 halfv = normalize(gl_LightSource[0].halfVector.xyz);
    vec3 normalz = normalize(normal);

    vec4 diffuse = gl_LightSource[0].diffuse* gl_FrontMaterial.diffuse* max(dot(normalz,lightori),0);
    vec4 specular = gl_LightSource[0].specular* gl_FrontMaterial.specular* pow( max(dot(normalz,halfv),0), gl_FrontMaterial.shininess);


    
    
    vec4 viewpos = vec4(worldpos, 1)*lightspace;
    //viewpos /= viewpos.w;
    float viewdepth = viewpos.z;
    float mapdepth = texture2D(depthTex, (viewpos.xy + 1) / 2).r;


    //viewdepth = smoothstep(-1, 1, viewdepth);
    mapdepth = smoothstep(-1, 1, mapdepth);

    float shadowK = smoothstep(-0.02,-0.01, mapdepth-viewdepth);
    matcolor += shadowK*vec4(0.5, 0.5, 0.5, 1);

    gl_FragColor = vec4(0,0,viewpos.z,1);      /*texture2D(baseTex, texcoord)*matcolor;*/
    
}