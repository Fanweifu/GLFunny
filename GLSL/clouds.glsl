uniform float     iTime;
uniform vec3      viewport;             
uniform mat4      prjInvMat;
uniform mat4      mdlInvMat;
uniform sampler2D iChannel0;
void setupCamera(vec2 uv,out vec3 campos, out vec3 dir){
     vec4 camdir = vec4(uv,1,1)*prjInvMat;
     camdir = camdir/camdir.w;

     vec4 camv = vec4(0,0,0,1);
     camv = camv*mdlInvMat;
     campos = camv.xyz;

     vec4 dirp = camdir*mdlInvMat;
     dir = normalize((dirp-camv).xyz);
}

vec3 getSkyColor(vec3 e) {
    e.y = max(e.z,0.0);
    return vec3(pow(1.0-e.z,2.0), 1.0-e.z, 0.6+(1.0-e.z)*0.4);
}

#define FLATTEN .2
#define NUM_STEPS 70

vec3 sunLight  = normalize( vec3(  0.35, 0.22,  0.3 ) );
vec3 sunColour = vec3(1.0, .86, .7);

#define cloudLower 2400.0
#define cloudUpper 3800.0


//#define TEXTURE_NOISE


float gTime;
float cloudy;
vec2 add = vec2(1.0, 0.0);
#define MOD3 vec3(3.07965, 7.1235, 4.998784)


//--------------------------------------------------------------------------
// A new Hash from https://www.shadertoy.com/view/XlfGWN
float Hash(vec3 p)
{
	p  = fract(p /  MOD3);
    p += dot(p.xyz, p.yzx + 19.19);
    return fract(p.x * p.y * p.z);
}


//--------------------------------------------------------------------------
#ifdef TEXTURE_NOISE
float Noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = textureLod( iChannel0, (uv+ 0.5)/256.0, 0.0 ).yx;
	return mix( rg.x, rg.y, f.z );
}
#else
//--------------------------------------------------------------------------
float Noise(in vec3 p)
{
    vec3 i = floor(p);
	vec3 f = fract(p); 
	f *= f * (3.0-2.0*f);

    return mix(
		mix(mix(Hash(i  			), 	Hash(i + add.xyy),f.x),
			mix(Hash(i + add.yxy),		Hash(i + add.xxy),f.x),
			f.y),
		mix(mix(Hash(i + add.yyx),    	Hash(i + add.xyx),f.x),
			mix(Hash(i + add.yxx), 		Hash(i + add.xxx),f.x),
			f.y),
		f.z);
}
#endif

//--------------------------------------------------------------------------
float FBM( vec3 p )
{
	p *= .5;
    float f;
    f  = 0.5000   * Noise(p); p =  p * 3.02;
    f += 0.2500   * Noise(p); p =  p * 3.03;
    f += 0.1250   * Noise(p); p =  p * 3.01;
    f += 0.0625   * Noise(p); p =  p * 3.03;
	f += 0.03125  * Noise(p); p =  p * 3.02;
	f += 0.015625 * Noise(p);
    return f;
}


//--------------------------------------------------------------------------
float Map(vec3 p)
{
	float h = FBM(p);
	return h-cloudy-.42;
}

//--------------------------------------------------------------------------
// Grab all sky information for a given ray from camera
vec3 GetSky(in vec3 pos,in vec3 rd)
{
	float sunAmount = max( dot( rd, sunLight), 0.0 );
	// Do the blue and sun...	
	vec3  sky = mix(vec3(.1, .1, .4), vec3(.1, .45, .7), 1.0-pow(abs(rd.y), .5));
	sky = sky + sunColour * min(pow(sunAmount, 1500.0) * 2.0, 1.0);
	sky = sky + sunColour * min(pow(sunAmount, 10.0) * .75, 1.0);
	
	// Find the start and end of the cloud layer...
	float beg = ((cloudLower-pos.y)/rd.y);
	float end = ((cloudUpper-pos.y)/rd.y);
	// Start position...
	vec3 p = vec3(pos.x + rd.x * beg, cloudLower, pos.z + rd.z * beg);

	// Trace clouds through that layer...
	float d = 0.0;
	float add = (end-beg) / float(NUM_STEPS);
	vec4 sum = vec4(0.1, .1, .1, 0.0);
	// Horizon fog is just thicker clouds...
	vec4 col = vec4(0.0, 0.0, 0.0, pow(1.0-rd.y,30.) * .2);
	for (int i = 0; i < NUM_STEPS; i++)
	{
		if (sum.a >= 1.0) continue;
		vec3 pos = p + rd * d;
		float h = Map(pos * .001);
		col.a += max(-h, 0.0) * .10; 
		col.rgb = mix(vec3((pos.y-cloudLower)/((cloudUpper-cloudLower))) * col.a, sunColour, max(.3-col.a, 0.0) * .04);
		sum = sum + col*(1.0 - sum.a);
		d += add;
	}
	sum.xyz += min((1.-sum.a) * pow(sunAmount, 3.0), 1.0);
	sky = mix(sky, sum.xyz, sum.a);

	return clamp(sky, 0.0, 1.0);
}

//--------------------------------------------------------------------------
vec3 CameraPath( float t )
{
    return vec3(4000.0 * sin(.16*t), 0.0, 4000.0 * cos(.155*t) );
} 


//--------------------------------------------------------------------------
vec3 IntoSphere(vec2 uv)
{
	vec3 dir;
	uv = (-1.0 + 2.0 * uv);
	dir.x = uv.x;
	dir.z = uv.y;
	dir.y = sqrt(1.0-dir.x * dir.x  - dir.z*dir.z) * FLATTEN;
	if (length(dir) >= 1.0) return vec3(0.0, .001, .999);
	dir = normalize(dir);
	
	return dir;
}

//--------------------------------------------------------------------------
vec2 IntoCartesian(vec3 dir)
{
	vec2 uv;
	dir.y /= FLATTEN;
	dir = normalize(dir);
	uv.x = dir.x;
	uv.y = dir.z;
	uv = .5 + (.5 * uv);
	return uv;
}

//--------------------------------------------------------------------------


void main(){
	vec2 uv = gl_FragCoord.xy / viewport.xy;
    uv = uv*2.0-1.0;

    vec3 pos,ray ;

    setupCamera(uv,pos,ray);

    //campos = yztozy(campos);
    //dirword = yztozy(dirword);
    cloudy = (cos(iTime)-1)/2;
   
    vec3 col = getSkyColor(ray);
	gl_FragColor = vec4(col, 1.0);
}