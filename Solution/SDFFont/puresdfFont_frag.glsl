uniform sampler2D sdfTex;
uniform float antiFactor = 0.2;
vec4 foreColor = vec4(1,1,1,1);
vec4 backColor = vec4(0,0,0,1);
float markDist = 0.0;

varying   vec2 texcoord;

float sdfFunc(vec2 coord) {
    return (texture2D(sdfTex,coord).r - 0.5);//half is zero
}

float alpha(vec2 coord, float anti){
	float dist = sdfFunc(coord);
	return smoothstep(-anti,anti,dist);
}

vec4 render(vec2 coord, float anti){
	vec2 dx = dFdx(coord), dy = dFdy(coord);
	float qua = 0.25;
	float asum = alpha(coord+qua*(-dx-dy),anti)+alpha(coord+qua*(-dx+dy),anti)+alpha(coord+qua*(dx-dy),anti)+alpha(coord+qua*(dx+dy),anti);
	asum = asum/4.0;
	
	return vec4(foreColor.rgb,asum);
}

void main(){

	float dist = sdfFunc(texcoord);
	float fw = fwidth(dist)*antiFactor;
	
	gl_FragColor = render(texcoord,fw);

}
