#version 330 core

uniform sampler2D TextureSampler;

// recebe o vetor de cores do vertex shader
in vec3 Color;
// recebe as coordenadas de textura do vertex shader
in vec2 UV;


in vec3 Normal;
// envia as cores para buffer da memória de vídeo
out vec4 OutColor;

uniform vec3 LightDirection;
uniform float LightIntensity;


void main()
{	
	// renormalizar a normal para evitar problemas de interpolação linear
	vec3 N = normalize(Normal);

	// inverte direção da luz
	vec3 L = -normalize(LightDirection);

	float lambertian = max(dot(N, L), 0.0); // direção

	vec3 TextureColor = texture(TextureSampler, UV).rgb;

	// aplica as cores utilizando a interpolação
	// OutColor = vec4(Color, 1);
	// OutColor = vec4(Color, 1.0);
	// vec3 FinalColor =  Color *  TextureColor;
	// vec3 FinalColor =  ColorIntensity *  TextureColor;
	vec3 FinalColor =  TextureColor * LightIntensity * lambertian;
	// FinalColor = vec3(1.0f);
	// OutColor = vec4(FinalColor, 1.0);
	// OutColor = vec4(normalize(Normal), 1.0);
	OutColor = vec4(FinalColor, 1.0);
}