uniform float     iTime;
uniform vec2      viewport;
uniform vec3      cameraPos;
uniform vec4      worldLight;
uniform mat4      prjInvMat;
uniform mat4      mdlInvMat;

vec3 yztozy(vec3 p){
    return vec3(p.x,p.z,p.y);
}

vec2 coordToUV(vec2 coord){
    return 2.0*coord/viewport-1.0;
}

vec3 uvToWorldDir(vec2 uv){
     vec4 camdir = vec4(uv,1,1)*prjInvMat;
     camdir = camdir/camdir.w;
     vec4 dirp = camdir*mdlInvMat;
     return normalize(dirp.xyz-cameraPos);
}void main() {    vec2 uv = coordToUV(gl_FragCoord.xy);	       vec3 ray = uvToWorldDir(uv);    vec3 pos = cameraPos;    gl_FragColor = vec4(ray,1);}