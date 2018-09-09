#version 130

uniform float     time;
uniform vec2      viewport;//���ڷֱ���

//�����һ��[-1,1]
vec2 coord2uv(vec2 coord){
    return 2.0*coord/viewport.xy-1.0;
}

//ͨ��uv���㱾������ϵ�µ����߷���
vec3 uv2ray(vec2 uv){
     vec4 camdir = gl_ProjectionMatrixInverse*vec4(uv,1,1);
     camdir = camdir/camdir.w;//W��һ�� �ó����������ϵ�µĵ�
     vec3 dir = mat3(gl_ModelViewMatrixInverse)*vec3(camdir);
     return normalize(dir);
}

//��������ϵ�µ��ӵ�����
vec3 cameraPos(){
    return vec3(gl_ModelViewMatrixInverse*vec4(0,0,0,1));
}

//���㱾������ϵ�µĵƹⷽ��ƽ�й⣩
vec3 lightPos(){
	return normalize(mat3(gl_ModelViewMatrixInverse)*gl_LightSource[0].position.xyz);
}

//��������ϵ�µ��Դ����
vec3 spotLightPos(){    
    return vec3(gl_ModelViewMatrixInverse*gl_LightSource[0].position);
}
#define MAXSTEP 40 //���ƽ�����
#define TOLERANCE 0.0001 // ������Ҫ��С����ֵ���� 


float sdsphere(vec3 pos,float r){
    return length(pos)-r;
}

vec2 map(vec3 pos){
    return vec2(sdsphere(pos,1),0.5);
}

//����:�ӵ����꣬���߷���;
//���:�����Ͷ�Ӧ����ϵ����
bool marching(vec3 pos,vec3 ray,out vec3 closeing,out float material){
    int step =0;
    float mindist = 1;
    vec2 res;
    do{
        res = map(pos);
        if(res.x<mindist){
            mindist = res.x;
            material = res.y;
            closeing = pos;
        }
        
        pos+= ray*res.x;       
        step++;
    }while(res.x>TOLERANCE&&step<=MAXSTEP);

    
    return step<=MAXSTEP;
}

float procDepth(vec3 localPos){
    vec4 frag = gl_ModelViewProjectionMatrix*vec4(localPos,1);
    frag/=frag.w;
    return (frag.z+1)/2;
}

#define NORMALESP 0.001
vec3 procNormal(vec3 marchpt)
{
    float dist = map(marchpt).x;

    return normalize(vec3(
        dist-map(marchpt-vec3(NORMALESP,0,0)).x,
        dist-map(marchpt-vec3(0,NORMALESP,0)).x,
        dist-map(marchpt-vec3(0,0,NORMALESP)).x
    ));
}

vec3 lightModel(vec3 eye,vec3 normal, vec3 light,float material){
	float NL = max(dot(normal,light),0);
	float RL = max(dot(eye,reflect(light,normal)),0);
	return vec3(material*0.1)+vec3(material)*NL+ vec3(material)*pow(RL,10);
}

//

void main(){
    vec2 uv = coord2uv(gl_FragCoord.xy);
    vec3 ro = cameraPos();
    vec3 rd = uv2ray(uv);

    vec3 nearest;
    float material;
      
    if(marching(ro,rd,nearest,material)){
		vec3 normal = procNormal(nearest);
        gl_FragDepth = procDepth(nearest);
        gl_FragColor = vec4(lightModel(rd,normal,lightPos(),material)*normal,1);
    }else{
       discard;
    }
}


