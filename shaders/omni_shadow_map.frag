#version 330
in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float len = length(fragPos.xyz - lightPos);
	len /= farPlane;
	gl_FragDepth = len;
}