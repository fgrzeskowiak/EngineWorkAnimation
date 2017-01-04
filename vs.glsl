//	shadertype=<glsl>
#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;

attribute  vec3 inPosition;
attribute  vec3 inTexCoord;
attribute  vec3 inNormal;

varying  vec3 vNormal;
varying  vec2 texCoord;

smooth out vec3 vEyeSpacePos;
smooth out vec3 vWorldPos;

void main()
{	
	vec4 vEyeSpacePosVertex = viewMatrix * modelMatrix *vec4(inPosition, 1.0);
	gl_Position = projectionMatrix*vEyeSpacePosVertex;

	vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
	vNormal = vRes.xyz;
	vEyeSpacePos = vEyeSpacePosVertex.xyz;

	texCoord = vec2(inTexCoord[0], inTexCoord[1]);
	vec4 vWorldPosVertex = modelMatrix *vec4(inPosition, 1.0);
	vWorldPos = vWorldPosVertex.xyz;
}