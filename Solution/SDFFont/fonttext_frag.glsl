uniform sampler2D  baseTex;



void main(){
	vec4 clr = texture2D(baseTex,gl_TexCoord[0].xy);
	gl_FragColor = vec4(clr.rgb*clr.a,1);
}