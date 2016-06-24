uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 eyePos;
uniform vec3 lightPos;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 tangent;

varying vec2 v_texCoord;
varying vec3 v_lightDirTS;
varying vec3 v_viewTS;

void main() {
   vec4 posWS = worldMat * vec4(position, 1.0);
   gl_Position = projMat * viewMat * posWS;
   v_texCoord = texCoord;

   vec3 nor = mat3(worldMat) * normal;
   vec3 tan = mat3(worldMat) * normalize(tangent);
   vec3 bin = cross(nor, tan);
   mat3 worldToTangentSpace = mat3(tan.x, bin.x, nor.x,
                                   tan.y, bin.y, nor.y,
                                   tan.z, bin.z, nor.z);

   v_lightDirTS = worldToTangentSpace * (lightPos - posWS.xyz);
   v_viewTS = worldToTangentSpace * (eyePos - posWS.xyz);
}