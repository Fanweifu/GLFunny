#version 330 compatibility
out vec4 fragLight;
out vec2 texcoord;
out vec3 normal; 

uniform mat4 cameraViewInv;
uniform mat4 lightSpace;
void main(){
    


    vec4 posInCam = gl_ModelViewMatrix*gl_Vertex; //相机坐标值;
    vec4 worldPos = cameraViewInv*posInCam;
    fragLight = lightSpace*worldPos;
    

    gl_Position = gl_ProjectionMatrix * posInCam;//相机坐标到投影空间;
    
    normal = gl_NormalMatrix*gl_Normal;
    texcoord = gl_MultiTexCoord0.xy;
}