uniform sampler2D tex;
varying vec2 texCoord;
varying vec3 normal;
varying vec3 position;
vec3 lightModel(vec3 normal,float material){
	vec3 light = gl_LightSource[0].position.xyz;
	float NL = max(dot(normal,light),0);
	vec3 halfVec = gl_LightSource[0].halfVector.xyz;
	float RL = max(dot(normal,halfVec),0);
	return vec3(material)*NL+ vec3(material)*pow(RL,9);
}

void main(){
	vec3 pdx = dFdx(position);
	vec3 pdy = dFdy(position);
	vec3 normal = normalize(cross(pdx,pdy));

	vec3 leffect = lightModel(normal,1);

	vec4 color = texture2D(tex,texCoord);
	gl_FragColor = vec4(leffect,1);

}