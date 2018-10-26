#version 120

uniform sampler2D sdfTex;
uniform vec4 backColor = vec4(0,0,0,1);//背景色
uniform vec4 foreColor = vec4(1,0,0,1);//前景色（填充）

uniform bool stroke = false;//使用描边？
uniform vec4 strokeColor = vec4(1,1,1,1);//描边颜色
uniform bool strokeGlow = false;//描边渐变？

uniform bool shadow = false; //使用阴影？
uniform bool shadowGlow = false;//使用阴影渐变?
uniform vec2 shadowOffest = vec2(0.01);//阴影偏移
uniform vec4 shadowColor = vec4(0,0,0,1);//阴影颜色

uniform float strokeWidth = 0.02;// 描边宽度
uniform float markDist = 0.0;// 填充距离  控制字体粗细
//uniform float smoothDelta = 0.002;// 抗锯齿因子

varying vec2 texcoord;

//#define NS 2

float sdfFunc(vec2 coord){
	return -(texture2D(sdfTex,coord).r-0.5);//half is zero
}


vec4 render(vec2 coord) 
{
    float d = sdfFunc(coord);
	float anti = fwidth(d);
	vec4 color = backColor;

	
	float fontDist = stroke? markDist+strokeWidth:markDist;

	if(shadow){
		float shwd = sdfFunc(texcoord-shadowOffest);
		float shwanti = fwidth(shwd);
		if(shadowGlow) color = mix(shadowColor,color,smoothstep(length(shadowOffest),0,shwd-fontDist));
		else color = mix(shadowColor,color,smoothstep(-shwanti,shwanti,shwd-fontDist));

	}


	if(stroke){
		if(strokeGlow) color = mix(strokeColor,color,smoothstep(0,strokeWidth,d-markDist));
		else color = mix(strokeColor,color,smoothstep(-anti,anti,d-fontDist));
	}


	color = mix(foreColor,color,smoothstep(-anti,anti,d-markDist));
	
	return color;
}



void main(){

    vec2 dx = dFdx(texcoord),dy = dFdy(texcoord);
	vec4 outcolor;
#ifdef NS
	for(int i=0;i<NS;i++){
		for(int j=0;j<NS;j++){
			vec2 pix = vec2(i,j)/NS-0.5;
			outcolor+=render(texcoord+pix*vec2(dx+dy));
		}
    }
	outcolor/=(NS*NS);
#else 
	outcolor = render(texcoord);
#endif

	gl_FragColor = outcolor;
}