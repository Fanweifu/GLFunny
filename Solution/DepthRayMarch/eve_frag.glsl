#version 130
uniform float     time;
uniform vec2      viewport;//窗口分辨率

//坐标归一到[-1,1]
vec2 coord2uv(vec2 coord){
    return 2.0*coord/viewport.xy-1.0;
}

//通过uv计算本地坐标系下的射线方向
vec3 uv2ray(vec2 uv){
     vec4 camdir = gl_ProjectionMatrixInverse*vec4(uv,1,1);
     camdir = camdir/camdir.w;//W归一后 得出相机坐标体系下的点
     vec3 dir = mat3(gl_ModelViewMatrixInverse)*vec3(camdir);
     return normalize(dir);
}

//本地坐标系下的视点坐标
vec3 cameraPos(){
    return vec3(gl_ModelViewMatrixInverse*vec4(0,0,0,1));
}

//计算本地坐标系下的灯光方向（平行光）
vec3 lightPos(){
	return normalize(mat3(gl_ModelViewMatrixInverse)*gl_LightSource[0].position.xyz);
}

//本地坐标系下点光源坐标
vec3 spotLightPos(){    
    return vec3(gl_ModelViewMatrixInverse*gl_LightSource[0].position);
}

#define MAXSTEP 40 //最大逼近步数
#define TOLERANCE 0.0001 // 距离需要减小到该值以下 
#define NORMALESP 0.001





#ifdef HIGH_QUALITY
#define NS 8
#else
#define NS 2
#endif


//------------ primitives ------------


float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
	vec3 pa = p-a, ba = b-a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	return length( pa - ba*h ) - r;
}

float sdSphere( vec3 p, vec4 s )
{
    return length(p-s.xyz) - s.w;
}

float sdEllipsoid( vec3 p, vec3 c, vec3 r )
{
    return (length( (p-c)/r ) - 1.0) * min(min(r.x,r.y),r.z);
}

float udRoundBox( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}

//------------ operators ------------

float smin( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

float smax( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( a, b, h ) + k*h*(1.0-h);
}

vec2 minx(vec2 a, vec2 b )
{
    return (a.x<b.x)?a:b;
}

//------------ maths ------------

mat4 matRotate( in vec3 xyz )
{
    vec3 si = sin(xyz);
    vec3 co = cos(xyz);

	return mat4( co.y*co.z,                co.y*si.z,               -si.y,       0.0,
                 si.x*si.y*co.z-co.x*si.z, si.x*si.y*si.z+co.x*co.z, si.x*co.y,  0.0,
                 co.x*si.y*co.z+si.x*si.z, co.x*si.y*si.z-si.x*co.z, co.x*co.y,  0.0,
			     0.0,                      0.0,                      0.0,        1.0 );
}

mat4 matTranslate( float x, float y, float z )
{
    return mat4( 1.0, 0.0, 0.0, 0.0,
				 0.0, 1.0, 0.0, 0.0,
				 0.0, 0.0, 1.0, 0.0,
				 x,   y,   z,   1.0 );
}

mat4 matInverse( in mat4 m )
{
	return mat4(
        m[0][0], m[1][0], m[2][0], 0.0,
        m[0][1], m[1][1], m[2][1], 0.0,
        m[0][2], m[1][2], m[2][2], 0.0,
        -dot(m[0].xyz,m[3].xyz),
        -dot(m[1].xyz,m[3].xyz),
        -dot(m[2].xyz,m[3].xyz),
        1.0 );
}

//--------------------------------------------

float hash( float n )
{
    return fract(sin(n)*1751.5453);
}

//--------------------------------------------


mat4 mtxHead;

vec2 map( vec3 p )
{
    vec3 q = p; q.x = abs(q.x);
    vec3 hp = (mtxHead*vec4(p,1.0)).xyz;

    // body
    p.yz = mat2(0.98,-0.2,0.2,0.98)*p.yz;
    
    float d1 = sdEllipsoid( p, vec3(0.0,0.15,0.0), vec3(0.41,0.83,0.41) );
    float d2 = sdSphere( p, vec4(0.0,1.4,0.0,1.2) );
    float d9 = sdCapsule( q, vec3(0.5,-0.6,0.02), vec3(0.45,0.07,-0.02), 0.15 );
    float body = smax( d1, -d2, 0.03 );
    body = smax( body, -d9, 0.01 );
    
    
    // head
    float d3 = sdSphere( hp, vec4(0.0,0.0,0.0,0.4) );
    float d4 = sdSphere( hp, vec4(0.0,0.45,0.008,0.65) );
    float head = smax( d3, d4, 0.05 );
    // head hole    
    float d5 = sdEllipsoid( hp, vec3(0.0,0.0,0.4), vec3(0.48,0.3,0.2) );
    //d5 = smax( d5, sdSphere( hp, vec4(0.0,0.34,0.2, 0.46 )), 0.07 );
    d5 = smax( d5, sdSphere( hp, vec4(0.0,0.49,0.2, 0.6 )), 0.07 );
    head = smax( head, -d5, 0.025 );
    
    // face
    float d6 = sdSphere( hp, vec4(0.0,0.04,-0.05,0.41) );
    float m2 = smax(d6,d5,0.01);

    
    // arm
    float d7 = sdEllipsoid( q, vec3(0.5,-0.15,0.0), vec3(0.1,0.35,0.1) );
    float d8 = sdEllipsoid( q, vec3(0.54,-0.15,0.0), vec3(0.1,0.35,0.1) );
//    d8 = sdCapsule( q, vec3(0.6,-0.6,0.02), vec3(0.55,0.09,-0.02), 0.15 );
    d7 = max( d7, d8 );
    float arm = d7;
    

    float m1 = min( min( body, head ), arm );
                        
    return minx( vec2(m1,0.0),
                 vec2(m2,1.0) );
}

bool marching(vec3 pos,vec3 ray,out float nearDis,out float material){
    int step =0;
    float mindist = 1,accDis = 0;
    vec2 res;
    do{
        res = map(pos+accDis*ray);
		accDis += res.x;
        if(res.x<mindist){
            mindist = res.x;
            material = res.y;
            nearDis = accDis;
        }
        step++;
    }while(res.x>TOLERANCE&&step<=MAXSTEP);

    
    return step<=MAXSTEP;
}

float procDepth(vec3 localPos){
    vec4 frag = gl_ModelViewProjectionMatrix*vec4(localPos,1);
    frag/=frag.w;
    return (frag.z+1)/2;
}

vec3 procNormal(vec3 marchpt)
{
    float dist = map(marchpt).x;

    return normalize(vec3(
        dist-map(marchpt-vec3(NORMALESP,0,0)).x,
        dist-map(marchpt-vec3(0,NORMALESP,0)).x,
        dist-map(marchpt-vec3(0,0,NORMALESP)).x
    ));
}




float calcAO( in vec3 pos, in vec3 nor )
{
	float occ = 0.0;
    for( int i=0; i<8; i++ )
    {
        //float h = 0.01 + 0.22*float(i)/7.0;
        float h = 0.01 + 0.21*float(i)/7.0;
        occ += (h-map( pos + h*nor )).x;
    }
    return clamp( 1.0 - 5.8*occ/8.0, 0.0, 1.0 );    
}

float calcSoftshadow( in vec3 ro, in vec3 rd, float k )
{
    float res = 1.0;
    float t = 0.01;
#ifdef HIGH_QUALITY
    for( int i=0; i<64; i++ )
#else
    for( int i=0; i<16; i++ )
#endif    
    {
        float h = map(ro + rd*t ).x;
        res = min( res, smoothstep(0.0,1.0,k*h/t) );
#ifdef HIGH_QUALITY
        t += clamp( h, 0.01, 0.025 );
#else        
        t += clamp( h, 0.04, 0.1 );
#endif        
		if( res<0.01 ) break;
    }
    
    return clamp(res,0.0,1.0);
}


vec3 shade( in vec3 ro, in vec3 rd, float t, float m )
{
    vec3 pos = ro + t*rd;
    vec3 nor = procNormal(pos);
    vec3 ref = reflect( rd, nor );
    vec3 sundir = lightPos();
    vec3 mate = vec3(1.0,1.0,1.0);
    
    if( m<0.5 )
    {
        mate = vec3(0.7);
    }
	else
    {
        mate = vec3(0.0,0.0,0.0);
        
        vec2 uv = (mtxHead * vec4(pos,1.0)).xy;
        //uv.x += 0.004*sin(500.0*uv.y);

        uv.x = abs(uv.x);
        uv -= vec2(0.11,0.565-0.5);
        vec2 st  = uv;
        uv = mat2(0.9,-0.4,0.4,0.9)*uv;
        vec3 eye = vec3(0.02,0.2,0.9)*1.9;
        eye *= 0.9 + 0.1*sin(512.0*st.x + sin(512.0*st.y));
        
        float eyesN = 1.0-smoothstep( 0.03, 0.06, length(uv * vec2(0.7,1.0)) );
        float eyesB = 1.0-smoothstep( 0.00,0.008,abs(st.y));
        float bl = smoothstep( 0.9,0.91, sin(10.0*time)*sin(3.0*time) );
        float eyes = mix( eyesN, eyesB, bl );
        mate = mix( mate, eye, eyes );
        
        mate += (0.01+mate)*0.9*smoothstep(-0.1,0.1,sin(st.y*400.0));
    }
    
    vec3 hal = normalize( sundir - rd );
    
    float fre = clamp( 1.0 + dot(nor,rd), 0.0, 1.0 );
    float occ = calcAO( pos, nor );

    float bak = clamp( dot(nor,normalize(vec3(-sundir.x,0.0,-sundir.z))), 0.0, 1.0 );
    float dif = clamp( dot(nor,sundir), 0.0, 1.0 );
    //float dif = clamp( dot(nor,lig)*0.8+0.2, 0.0, 1.0 );
    float spe = clamp( dot(nor,hal), 0.0, 1.0 );
    float sha = calcSoftshadow( pos, sundir, 8.0 ); 
    dif *= sha;

    vec3 col = 1.4*vec3(1.0,0.9,0.8)*dif +  0.8*vec3(0.2,0.28,0.35)*occ;
    col += vec3(1.2,1.0,0.8)*fre*(0.3+0.7*dif)*occ*3.5;
    //col += vec3(1.2,1.0,0.8)*fre*fre*(0.6+0.4*dif)*(0.6+0.4*occ)*2.0;
    col += vec3(0.4,0.3,0.2)*bak*occ;
    
    col *= mate;
    
    col += 3.5*vec3(0.3,0.4,0.5) * smoothstep( -0.1, 0.1, ref.y ) * (0.04 + 0.96*pow( fre, 5.0 )) * occ;
    col += 2.0*vec3(1.0)*pow( spe, 64.0 ) * (0.2 + 0.8*pow( fre, 5.0 )) * (occ*dif);
    
    col = pow( col, vec3(0.8,1.0,0.9) );

    col *= mix( vec3(0.3,0.2,0.1), vec3(1.0), smoothstep(-1.0,0.4,pos.y) );
    return col;        
}


vec3 render(vec3 ro, vec3 rd , out float depth)
{
    
    float matri,clstep;
	
    if(!marching( ro, rd, clstep, matri )){
	    discard;
		return vec3(0);
	}
    
    vec3 pt = ro+rd*clstep;
	depth = procDepth(pt.xzy);
	
    vec3 col = shade(ro,rd,clstep,matri);
    

    return pow( col, vec3(0.45) );
}



vec3 title( in vec3 col, in vec2 p )
{
    return col;
}


void main()
{

    float rt = mod( time, 15.0 );
    float zo = step( 11.0, rt );
    float zf = clamp( (rt-11.0)/(15.0-11.0), 0.0, 1.0 );

    float an1 = sin(0.5*time*1.0+0.4)*(1.0-zo);
    float an2 = sin(0.5*time*1.3+0.0)*(1.0-zo);
    float an3 = sin(2.0*time);
    an1 = an1*an1*an1;
    an2 = an2*an2*an2;
    
    mat4 hRot = matRotate( vec3(-0.2*an1*an1*an1 - 0.1*zo,-1.0*an2*an2*an2, 0.0) );
    mat4 hTra = matTranslate( 0.0,0.5+0.015*an3-0.02*zo,0.11 );

    mtxHead = matInverse( hTra * hRot );

    vec2 q = gl_FragCoord.xy/viewport;

    //float an4 = 0.3 + 0.2*sin(0.04*time);
    //vec3 ro = vec3( -5.2*sin(an4), 0.0+0.05*zo, 5.2*cos(an4) );
    
    //vec3 ta = vec3(0.0,0.2+0.3*zo,0.0);
    //ta += 0.02*cos( 1.0*time + vec3(0.0,2.0,3.0) )*(1.0-0.25*zo);
    //ro += 0.02*cos( 1.0*time + vec3(1.0,3.5,5.0) )*(1.0-0.25*zo);
    //mat3 ca = setCamera( ro, ta, 0.0 );
    //float fl = 1.5 * (2.2 + 2.5*zo*(1.0+0.25*zf));
    

    vec3 col = vec3(0.0);
	vec3 ro = cameraPos();
	float depth  = 0;
    for( int j=0; j<NS; j++ )
    for( int i=0; i<NS; i++ )
    {    
        vec2 uv = coord2uv(gl_FragCoord.xy+(vec2(i,j)/NS-0.5));

        vec3 rd = uv2ray(uv);
		float dpt = 0;
        col += render( ro.xzy , rd.xzy, dpt );
		depth+=dpt;
    }
    col /= float(NS*NS);
    depth/= float(NS*NS); 

    //// saturate
    //col = mix( col, vec3(dot(col,vec3(0.333))), -0.1 );

    //// vignette
    //col *= 0.2 + 0.8*pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 );

    //// letterbox
    //col *= 1.0 - smoothstep( 0.4, 0.41, abs(q.y-0.5) );

    //// flicker
    //col *= 1.0 + 0.015*fract( 17.1*sin( 13.1*floor(12.0*time) ));
    
    //col = title(col,q);
    
    gl_FragColor = vec4(col,1.0);
	gl_FragDepth = depth;
}