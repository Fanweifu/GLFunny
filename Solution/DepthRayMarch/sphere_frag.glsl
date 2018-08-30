#version 130

uniform float     time;
uniform vec2      viewport;

const float radiu = 0.5;

vec2 coord2uv(vec2 coord) {
	return 2.0*coord / viewport.xy - 1.0;
}

vec3 uv2ray(vec2 uv) {
	vec4 camdir = gl_ProjectionMatrixInverse*vec4(uv, 1, 1);
	camdir = camdir / camdir.w;
	vec3 dir = mat3(gl_ModelViewMatrixInverse)*vec3(camdir);
	return normalize(dir);
}

vec3 lightPos() {
	return normalize(mat3(gl_ModelViewMatrixInverse)*gl_LightSource[0].position.xyz);
}

vec3 eyePosition() {
	return vec3(gl_ModelViewMatrixInverse*vec4(0, 0, 0, 1));
}

///rayTrace

const float maxPrecis = 0.0001;
const float minDiscard = 0.3;
const int maxStep = 40;

float sdsphere(vec3 pos, float radiu) {
	return abs(length(pos) - radiu);
}

vec2 rayMarch(vec3 pos) {
	return vec2(sdsphere(pos, radiu), 1);
}

//#define Smooth 1

vec3 lightModel(vec3 eye, vec3 normal, vec3 light) {
	float NL = max(dot(normal, light), 0);
	float RL = max(dot(eye, reflect(light, normal)), 0);
	return vec3(0.5) + vec3(0.5)*NL + vec3(0.5)*pow(RL, 1);
}


void main() {
	vec4 color;
	float depth = 0;
#ifdef Smooth
	for (int i = -Smooth; i <= Smooth; i++) {
		for (int j = -Smooth; j <= Smooth; j++) {
			vec2 frag = gl_FragCoord.xy + vec2(i, j);
#else
	vec2 frag = gl_FragCoord.xy;
#endif
	vec2 uv = coord2uv(frag);
	vec3 dir = uv2ray(uv);

	vec2 march;
	float stepNum = 0;
	float mindist = 1;
	vec3 pos = eyePosition();
	vec3 nestpt = pos;
	do {
		march = rayMarch(pos);
		if (mindist>march.x) {
			nestpt = pos;
			mindist = march.x;
		}
		pos += march.x*dir;
		stepNum++;
	} while (march.x>maxPrecis&&stepNum<maxStep);

	float alpha = 1;

	float discarddist = minDiscard*(sin(time) / 2.0 + 1.8);

	if (stepNum >= maxStep) {
		if (mindist>discarddist) {
			discard;
			return;
		}
		else alpha = pow(smoothstep(-discarddist, -maxPrecis, -mindist), 2);
	}

	float dist = rayMarch(pos).x;
	vec3 sunlight = lightPos();
	vec3 normal = stepNum >= maxStep ? sunlight : normalize(pos);/*normalize(vec3((dist-rayMarch(pos-vec3(maxPrecis,0,0))).x,
																 (dist-rayMarch(pos-vec3(0,maxPrecis,0))).x,
																 (dist-rayMarch(pos-vec3(0,0,maxPrecis))).x));*/
	color += vec4(lightModel(dir, normal, sunlight), alpha);

	vec4 fragpos = gl_ModelViewProjectionMatrix*vec4(nestpt, 1);
	fragpos /= fragpos.w;
	depth += fragpos.z;

#ifdef Smooth
		}
	}
	int cnt = (2 * Smooth + 1)*(2 * Smooth + 1);
	color /= cnt;
	depth /= cnt;
#endif
	depth = clamp(depth, 0, 1);
	gl_FragDepth = depth;
	gl_FragColor = vec4(vec3(normalize(nestpt))*color.xyz, alpha);
}
