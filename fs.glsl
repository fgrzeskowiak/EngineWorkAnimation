//	shadertype=<glsl>
#version 330

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
	vec4 position;
	vec3 intensities; //a.k.a the color of the light
	//float attenuation;
	float ambientCoefficient;
	float fConstant;
	float fLinear;
	float fExp;
} allLights[MAX_LIGHTS];

varying vec3 vNormal;
varying vec2 texCoord;
uniform vec3 LightDirection;
uniform sampler2D gSampler;
uniform float colorAlpha;

smooth in vec3 vWorldPos;
smooth in vec3 vEyeSpacePos;

out vec4 outputColor;

vec4 vTexColor;
vec4 transparency;

vec4 ApplyLight(Light light, vec3 normal, vec4 TexColor, vec4 transparency, vec3 worldPos) {
	

	vec3 surfaceToLight;
	float fDist;
	float fDiffuse;
	float fAttTotal;
	if (light.position.w == 1.0) {
		//point light
		surfaceToLight = worldPos - light.position.xyz;
		fDist = length(surfaceToLight);
		surfaceToLight = normalize(surfaceToLight);
		fDiffuse = max(0.0, dot(normal, -surfaceToLight));
		fAttTotal = light.fConstant + light.fLinear*fDist + light.fExp*fDist*fDist;
		return transparency * TexColor  * vec4(light.intensities, 1.0)*(light.ambientCoefficient + fDiffuse) / fAttTotal;
		
	}
	else {
		fDiffuse = max(0.0, dot(normal, -LightDirection));	  
		return transparency * TexColor*vec4(light.intensities*(light.ambientCoefficient + fDiffuse), 1.0);
	}
}

void main()
{	  	
		vec3 vNormalized = normalize(vNormal);
		vec4 linearColor = vec4(0);
		vTexColor = texture2D(gSampler, texCoord);
		transparency = vec4(1.0, 1.0, 1.0, colorAlpha);
		for (int i = 0; i < numLights; i++){
			linearColor += ApplyLight(allLights[i], vNormalized, vTexColor, transparency, vWorldPos);
		}
	
		outputColor = linearColor;
		
}