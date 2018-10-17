#version 130

uniform vec2 viewport;
uniform vec3 backColor = vec3(1,1,1);
uniform vec3 lineColor = vec3(0,0,0);
uniform vec3 fillColor = vec3(0,1,1);
uniform float lineWidth = 2;

float sdfCircle(vec2 coord, vec2 center, float radius)
{
	return distance(coord,center) - radius;
}

vec3 render(float d) 
{
	float anti = fwidth(d);
	vec3 color = backColor;
	color = mix(lineColor,color,smoothstep(-anti, anti, d-lineWidth));
	color = mix(fillColor,color,smoothstep(-anti, anti, d));

	return color;
}

void main() 
{
	float a = sdfCircle(gl_FragCoord.xy, vec2(0.5, 0.5)* viewport , 100);
	
	gl_FragColor = vec4(render(a),1);
}