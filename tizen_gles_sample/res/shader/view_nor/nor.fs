precision mediump float;

uniform sampler2D s_tex0;       // base map
uniform sampler2D s_texNor;     // normal map

uniform vec3 materialSpec, materialAmbi, materialEmit;  // Md, Ms, Ma, Me
uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;        // Sd, Ss, Sa

varying vec2 v_texCoord;             // texture coordinates
varying vec3 v_lightDirTS, v_viewTS;    // light direction in world space, view direction in world space

struct Material {
    float sh;                        // shininess (specular power)
    vec3 diff, spec, ambi, emit;     // material colors
};

struct Light {
    vec3 dir, diff, spec, ambi;      // light direction and colors
};

// phong shading
vec3 phongLight(vec3 view, vec3 normal, // view direction and normal
				Material M, Light S) {  // material and source light
    float diff = max(dot(normal, S.dir), 0.0);
	vec3 refl = 2.0 * normal * dot(normal, S.dir) - S.dir; // reflection vector
	float spec = 0.0;
	if(diff > 0.0) spec = pow(max(dot(refl, view), 0.0), M.sh); // specular computation

	vec3 sum = vec3(0.0);
	sum += diff * S.diff * M.diff;      // add diffuse term
	sum += spec * S.spec * M.spec;      // add specular term
	sum += S.ambi * M.ambi + M.emit;    // add ambient and emissive term

	return sum;
}

void main() {
    vec3 materialDiff = texture2D(s_tex0, v_texCoord).xyz;
    vec3 norTS = normalize(texture2D(s_texNor, v_texCoord).xyz * 2.0 - vec3(1.0));
    Material material 
        = Material(materialSh, materialDiff, materialSpec, materialAmbi, materialEmit);
    Light source 
        = Light(normalize(v_lightDirTS), sourceDiff, sourceSpec, sourceAmbi);
    vec3 color 
        = phongLight(normalize(v_viewTS), normalize(norTS), material, source);
    gl_FragColor = vec4(color, 1.0);
}