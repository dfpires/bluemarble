#version 330 core

layout (location = 0) in vec3 InPosition;

void main()
{	// construindo um vec4 (coordenada normalizada) a partir de um vec3 (veio do programa)
	// InPosition conterá as coordenadas do Triangle
	gl_Position = vec4(InPosition, 1.0); // 1 é uma posição

	}