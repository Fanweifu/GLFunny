
uniform sampler2D sdfTex;
uniform bool smoothStep = true;
uniform bool usefwidth = true;
uniform vec4 backColor = vec4(1,1,1,0);//背景色
uniform vec4 foreColor = vec4(0,1,1,1);//前景色（填充）


uniform bool stroke = false;//使用描边？
uniform vec4 strokeColor = vec4(1,1,1,1);//描边颜色
uniform bool strokeGlow = false;//描边渐变？

uniform bool shadow = false; //使用阴影？
uniform bool shadowGlow = false;//使用阴影渐变?
uniform vec2 shadowOffest = vec2(0.01);//阴影偏移
uniform vec4 shadowColor = vec4(0,0,0,1);//阴影颜色
uniform float distScale = 8.0;
uniform float antiFactor = 1.0;
uniform float strokeWidth = 0.02;// 描边宽度
uniform float markDist = 0.0;// 填充距离  控制字体粗细

varying vec2 texcoord;


float sdfFunc(vec2 coord){
	return -(texture2D(sdfTex,coord).r-0.5)/distScale;//half is zero
}


vec4 render(vec2 coord) 
{
    float d = sdfFunc(coord);
	
	float anti = fwidth(d);
	anti*=antiFactor;
	vec4 color = backColor;

	float fontDist = stroke? markDist+strokeWidth:markDist;

	if(shadow){
		float shwd = sdfFunc(coord-shadowOffest);
		float shwanti = fwidth(shwd);
		if(shadowGlow) color = mix(shadowColor,color,smoothstep(-length(shadowOffest),0,shwd-fontDist));
		else color = mix(shadowColor,color,smoothstep(-shwanti,shwanti,shwd-fontDist));

	}

	if(stroke){
		if(strokeGlow) color = mix(strokeColor,color,smoothstep(0,strokeWidth,d-markDist));
		else color = mix(strokeColor,color,smoothstep(-anti,anti,d-fontDist));
	}

	
	float mixf = smoothstep(-anti,anti,d-markDist);
	
	color = mix(foreColor,color,mixf);
	
	return color;
}

void main(){
	gl_FragColor = render(texcoord);
}