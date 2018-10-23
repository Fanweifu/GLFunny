#version 120

uniform sampler2D sdfTex;
uniform vec2 sdfSize = vec2(32,32);//sdf贴图分辨率
uniform vec4 backColor = vec4(0,0,0,1);//背景色
uniform vec4 foreColor = vec4(1,1,1,1);//前景色（填充）

uniform bool stroke = false;//使用描边？
uniform vec4 strokeColor = vec4(1,1,1,1);//描边颜色
uniform bool strokeGlow = true;//描边渐变？

uniform bool shadow = false; //使用阴影？
uniform bool shadowGlow = true;//使用阴影渐变?
uniform vec2 shadowOffest = vec2(0.003);//阴影偏移
uniform vec4 shadowColor = vec4(0,0,0,1);//阴影颜色

uniform float strokeWidth = 0.008;// 描边宽度
uniform float markDist = 0.003;// 填充距离  控制字体粗细
uniform float smoothDelta = 0.002;// 抗锯齿因子

varying vec2 texcoord;

#define NS 2

float sdfFunc(vec2 coord){
	return (texture2D(sdfTex,coord).r-0.5);//half is zero
}


vec4 render(vec2 coord) 
{
	float d = sdfFunc(coord) , anti = fwidth(d)/2;
	vec4 color = backColor;

	float fontDist = stroke? markDist+strokeWidth:markDist;

	if(shadow){
		float shwd = sdfFunc(texcoord+shadowOffest);
		if(shadowGlow) color = mix(shadowColor,color,smoothstep(-length(shadowOffest)*2,0,shwd-fontDist));
		else color = mix(shadowColor,color,smoothstep(-anti,anti,shwd-fontDist));
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
	for(int i=0;i<NS;i++){
		for(int j=0;j<NS;j++){
			vec2 pix = vec2(i,j)/NS-0.5;
			outcolor+=render(texcoord+pix*vec2(dx+dy));
		}
    }
	
	

	gl_FragColor = outcolor/(NS*NS);
}