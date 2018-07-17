#version 130

out vec3 worldpos;
out vec2 texcoord;
out vec3 normal;
void main(){
    worldpos = gl_Vertex.xyz;
    gl_Position = ftransform();

    normal = gl_NormalMatrix*gl_Normal;
    texcoord = gl_MultiTexCoord0.xy;
}