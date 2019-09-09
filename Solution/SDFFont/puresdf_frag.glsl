uniform sampler2D sdfTex;
vec4 foreColor = vec4(0,0,0,1);
vec4 backColor = vec4(0,0,0,1);
uniform float markDelta = 0.4;

varying   vec2 texCoord;

float sdfFunc(vec2 coord) {
    return (texture2D(sdfTex,coord).a - 0.5);//half is zero
}

float alpha(vec2 coord, float anti){
	float dist = sdfFunc(coord);
	return smoothstep(-anti,anti,dist);
}

vec4 render(vec2 coord, float anti){
	vec2 dx = dFdx(coord), dy = dFdy(coord);
	float qua = 0.25;
	float asum = alpha(coord+qua*(-dx-dy),anti)+alpha(coord+qua*(-dx+dy),anti)+alpha(coord+qua*(dx-dy),anti)+alpha(coord+qua*(dx+dy),anti);
	asum = asum*qua;
	
	return vec4(foreColor.rgb,asum);
}

void main(){

	float dist = sdfFunc(texCoord);
	float fw = fwidth(dist)*markDelta;
	gl_FragColor.a = render(texCoord,fw).a;
	gl_FragColor.rgb = texture2D(sdfTex,texCoord).rgb;

	gl_FragColor.rgb*= gl_FragColor.a;
}
