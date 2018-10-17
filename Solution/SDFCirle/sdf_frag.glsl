#version 120

uniform vec2 viewport;
uniform vec4 backColor = vec4(1,1,1,0);
uniform vec4 lineColor = vec4(0,0,0,1);
uniform vec4 fillColor = vec4(0,1,1,1);
uniform float lineWidth = 0.02;

varying vec2 texcoord;

float sdfCircle(vec2 coord, vec2 center, float radius)
{
	return distance(coord,center) - radius;
}

vec4 render(float d) 
{
	float anti = fwidth(d);
	vec4 color = backColor;
	color = mix(lineColor,color,smoothstep(-anti, anti, d-lineWidth));
	color = mix(fillColor,color,smoothstep(-anti, anti, d));

	return color;
}

void main() 
{
	float a = sdfCircle(texcoord, vec2(0.5,0.5) , 0.1);
	
	gl_FragColor = render(a);
}