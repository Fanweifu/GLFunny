uniform sampler2D  baseTex;

void main(){
	gl_FragColor = texture2D(baseTex,gl_TexCoord[0].xy);
}