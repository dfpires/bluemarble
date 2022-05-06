#version 330 core

uniform sampler2D TextureSampler;

// recebe o vetor de cores do vertex shader
in vec3 Color;
// recebe as coordenadas de textura do vertex shader
in vec2 UV;

// envia as cores para buffer da memória de vídeo
out vec4 OutColor;


void main()
{	
	vec3 TextureColor = texture(TextureSampler, UV).rgb;

	// aplica as cores utilizando a interpolação
	// OutColor = vec4(Color, 1);
	// OutColor = vec4(Color, 1.0);
	// float ColorIntensity = 0.5;
	// vec3 FinalColor =  Color *  TextureColor;
	// vec3 FinalColor =  ColorIntensity *  TextureColor;
	 OutColor = vec4(TextureColor, 1.0);
}