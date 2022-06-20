#version 330 core

// conteúdo do InPosition vem do main.cpp 
layout (location = 0) in vec3 InPosition;

// conteúdo do InNormal vem do main.cpp 
layout (location = 1) in vec3 InNormal; // iluminação

// conteúdo do InUV vem do main.cpp 
layout (location = 2) in vec3 InColor;

// conteúdo do InUV vem do main.cpp 
layout (location = 2) in vec2 InUV;

uniform mat4 NormalMatrix; // iluminação

// conteúdo do ModeViewProjection virá do main.cpp
uniform mat4 ModelViewProjection;

out vec3 Color;

out vec3 Normal;

out vec2 UV;

void main() {	

	Normal = vec3(NormalMatrix * vec4(InNormal, 0.0)); // iluminação

	Color = InColor;

	UV = InUV;
	// construindo um vec4 (coordenada normalizada) a partir de um vec3 (veio do programa)
	// InPosition conterá as coordenadas do Triangle
	// agora o triângulo não mais terá coordenada normalizada
	// as coordenadas são obtidas em função da matriz
	// de projeção e de visão
	gl_Position = ModelViewProjection * vec4(InPosition, 1.0); // 1 é uma posição

	}