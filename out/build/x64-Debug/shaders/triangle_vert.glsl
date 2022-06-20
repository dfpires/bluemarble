#version 330 core

// conte�do do InPosition vem do main.cpp 
layout (location = 0) in vec3 InPosition;

// conte�do do InNormal vem do main.cpp 
layout (location = 1) in vec3 InNormal; // ilumina��o

// conte�do do InUV vem do main.cpp 
layout (location = 2) in vec3 InColor;

// conte�do do InUV vem do main.cpp 
layout (location = 2) in vec2 InUV;

uniform mat4 NormalMatrix; // ilumina��o

// conte�do do ModeViewProjection vir� do main.cpp
uniform mat4 ModelViewProjection;

out vec3 Color;

out vec3 Normal;

out vec2 UV;

void main() {	

	Normal = vec3(NormalMatrix * vec4(InNormal, 0.0)); // ilumina��o

	Color = InColor;

	UV = InUV;
	// construindo um vec4 (coordenada normalizada) a partir de um vec3 (veio do programa)
	// InPosition conter� as coordenadas do Triangle
	// agora o tri�ngulo n�o mais ter� coordenada normalizada
	// as coordenadas s�o obtidas em fun��o da matriz
	// de proje��o e de vis�o
	gl_Position = ModelViewProjection * vec4(InPosition, 1.0); // 1 � uma posi��o

	}