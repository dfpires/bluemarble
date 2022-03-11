#include<iostream>

#define GLM_SWIZZLE

#include<glm/glm.hpp>
#include<glm/gtx/string_cast.hpp>


void Operations() {
	std::cout << std::endl;
	glm::vec3 P0{ 10.0f, 10.0f, 0.0f };
	glm::vec3 P1{ 10.0f, 10.0f, 10.0f };

	glm::vec3 R;

	// soma
	R = P0 + P1;
	std::cout << glm::to_string(R) << std::endl;

	// subtração
	R = P0 - P1;
	std::cout << glm::to_string(R) << std::endl;

	// scala
	R = P0 * 5.0f;
	std::cout << glm::to_string(R) << std::endl;
	R = P0 / 5.0f;
	std::cout << glm::to_string(R) << std::endl;

	// multiplicação
	R = P0 * P1;
	std::cout << glm::to_string(R) << std::endl;
	R = P0 / P1;
	std::cout << glm::to_string(R) << std::endl;

	// comprimento
	float L = glm::length(P0);
	std::cout << L << std::endl;
	std::cout << P0.length() << std::endl;

	// normalizar - retorna vetor de tamanho 1
	glm::vec3 Norm = glm::normalize(P0);

	// produto escalar (Dot Product) - cosseno do ângulo entre os vetores
	float Dot = glm::dot(P0, P1);

	// produto vetorial - cross product - area definida pelo paralelograma
	glm::vec3 Cross = glm::cross(P0, P1);

	// distância entre dois pontos
	float Distance = glm::distance(P0, P1);

	// vetor de refração - vetor de incidência e vetor normal
	glm::vec3 Refract = glm::refract(P0, Norm, 1.0f);

	// vetor de reflexão - vetor de incidência e vetor normal
	glm::vec3 Reflect = glm::reflect(P0, Norm);


}
void Swizzle() {
	std::cout << std::endl;
	glm::vec4 P{ 1, 2, 3, 4 };
	
	glm::vec3 Q = P.xxx;
	glm::vec3 R = P.xyx;
	glm::vec4 T = P.xyzw;
	glm::vec4 Y = P.xzzw;


	std::cout << glm::to_string(Y)  << std::endl;
}

void Components() {
	std::cout << std::endl;
	glm::vec3 P{ 1, 2, 3 };
	std::cout << "X: " << P.x << " Y: " << P.y << " Z: " << P.z << std::endl;
	std::cout << "R: " << P.r << " G: " << P.g << " B: " << P.b << std::endl;
	std::cout << "0: " << P[0] << " 1: " << P[1] << " 2: " << P[2] << std::endl;
}
void Constructors() {
	std::cout << std::endl;

	glm::vec2 Point0 { 10.0f, 10.0f };
	glm::vec3 Point1 { 10.0f, 0.0f, 0.0f };
	glm::vec4 Point2 { 10.0f, 10.0f, 0.0f, 0.0f };
	glm::ivec2 Point3 { 3, 4};
	glm::bvec3 Point4 { false, true, false };
	glm::dvec2 Point5 { 10.0, 10.0 };
	glm::vec3 Point6 { Point0, 1.0f };
	glm::vec2 Point7{ Point2 };

	std::cout << " Point 0 " << glm::to_string(Point0) << std::endl;
	std::cout << " Point 1 " << glm::to_string(Point1) << std::endl;
	std::cout << " Point 2 " << glm::to_string(Point2) << std::endl;
	std::cout << " Point 3 " << glm::to_string(Point3) << std::endl;
	std::cout << " Point 4 " << glm::to_string(Point4) << std::endl;
	std::cout << " Point 5 " << glm::to_string(Point5) << std::endl;
	std::cout << " Point 6 " << glm::to_string(Point6) << std::endl;
	std::cout << " Point 7 " << glm::to_string(Point7) << std::endl;
}

int main() {

	Constructors();
	Components();
	Swizzle();
	Operations();


	std::cout << "Hello World" << std::endl;
	return 0;
}