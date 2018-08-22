#version 330 compatibility
out vec4 fragLight;
out vec2 texcoord;
out vec3 normal; 

uniform mat4 cameraViewInv;
uniform mat4 lightSpace;
void main(){
    


    vec4 posInCam = gl_ModelViewMatrix*gl_Vertex; //�������ֵ;
    vec4 worldPos = cameraViewInv*posInCam;
    fragLight = lightSpace*worldPos;
    

    gl_Position = gl_ProjectionMatrix * posInCam;//������굽ͶӰ�ռ�;
    
    normal = gl_NormalMatrix*gl_Normal;
    texcoord = gl_MultiTexCoord0.xy;
}