#version 120

uniform vec4 backColor = vec4(1,1,1,0);
uniform vec4 lineColor = vec4(0,0,0,1);
uniform vec4 fillColor = vec4(0,1,1,1);
uniform float time;

varying vec2 texCoord;
varying vec3 normal;

float rect(vec2 p){
	vec2 box = vec2(0.4,0.3);
	vec2 d = abs(box) - abs(p);
    return  max(min(d.x,d.y),0.0) - length(min(d,vec2(0)));
}

float dist(vec2 p)
{
	return rect(p-0.5);
}

float alpha(float d){
    float s = fwidth(d);
	return smoothstep(-s,s,d);
}


vec4 baseLayer(vec2 p,vec3 c){
	float d = dist(p);
	float a = alpha(d);
	return vec4(c,a);
}

float strokeDist(vec2 p,float w){ return dist(p)+w; }
vec4 strokeLayer(vec2 p,vec3 c,float w){
      float d  = strokeDist(p,w);
      float a  = alpha(d);
      return  vec4(c,a);
}

float strokeHollow(vec2 p,float o,float i){
	return min(strokeDist(p,o),-strokeDist(p,i));
}
vec4 strokeHollowLayer(vec2 p,vec3 c,float o,float i){
	float d  = strokeHollow(p,o,i);
    float a  = alpha(d);
    return  vec4(c,a);
}

float blurAlpha(float d,float w){
	return smoothstep(-w,w,d);
}

vec4 blurLayer(vec2 p,vec3 c,float w){
	float d  = strokeHollow(p,0.02,-0.02);
	w = max(w,fwidth(d));
    float a  = blurAlpha(d,w);
    return  vec4(c,a);
}

float shadowDist(vec2 p,vec2 shift){
	return dist(p-shift);
}

vec4 shadowLayer(vec2 p,vec3 c,vec2 s,float w)
{
	float d = shadowDist(p,s);
	w = max(w,fwidth(d));
	float a = blurAlpha(d,w);
	return vec4(c,a);
}

void main() 
{
    vec4 shadowColor = shadowLayer(texCoord,vec3(0,0,0),vec2(0.02,0.02*time),0.03);
	vec4 baseColor = baseLayer(texCoord,vec3(0.5,1,1));
	gl_FragColor = mix(shadowColor,baseColor,baseColor.a);
}

/*vec4 repeat(vec2 p,vec3 c){
	float d  = max( mod(-dist(p),0.01), mod(dist(p),0.01))-0.01*0.75;
	float a = alpha(d);
	return vec4(c,a*a);
}*/