#version 330

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aTexCoord;

out vec4 fragLight;
out vec2 texcoord;
out vec3 normal;

uniform mat4 cameraViewInv;
uniform mat4 lightSpace;
void main(){
    


    vec4 posInCam = gl_ModelViewMatrix*vec4(aPos,1); //�������ֵ;
    vec4 worldPos = cameraViewInv*posInCam;
    fragLight = lightSpace*worldPos;
    

    gl_Position = gl_ProjectionMatrix * posInCam;//������굽ͶӰ�ռ�;
    
    normal = gl_NormalMatrix*aNormal;
    texcoord = aTexCoord; 
}