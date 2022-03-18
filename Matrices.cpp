#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

void PrintMatrix(const glm::mat4& M) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout
				<< std::setw(10)
				<< std::setprecision(4)
				<< std::fixed
				<< M[j][i] << " ";
		}
		std::cout << std::endl;
	}

}
void TranslationMatrix() {
	// pula linha
	std::cout << std::endl;
	// mostra mensagem
	std::cout << "Translation Matrix" << std::endl;
	// cria uma matriz identidade 4x4 - I 
	glm::mat4 I = glm::identity<glm::mat4>();
	// cria o vetor de translação
	glm::vec3 T(10, 10, 10);
	// matriz de translação
	glm::mat4 Translation = glm::translate(I, T);
	// imprime
	PrintMatrix(Translation);
//	std::cout << glm::to_string(M) << std::endl;

	// cria um vetor de posição
	glm::vec4 Position(10, 10, 10, 1); // posição
	// cria um vetor de direção
	glm::vec4 Direction(10, 10, 10, 0); // direção
	// aplica a translação no vetor de posição
	Position = Translation * Position;
	// aplica a translação no vetor de direção
	// aplicar translação no vetor de direção 
	// não tem efeito
	Direction = Translation * Direction;

	std::cout << glm::to_string(Position) << std::endl;
	// vetor não foi alterado com a translação
	std::cout << glm::to_string(Direction) << std::endl;
}

void ScaleMatrix() {
	// pula linha
	std::cout << std::endl;
	// mensagem
	std::cout << "Scale Matrix" << std::endl;
	// cria a matriz 4x4 identidade - I
	glm::mat4 I = glm::identity<glm::mat4>();
	// cria o vetor de escala
	// qtde que vamos escalar (dobrar em x, y e z)
	glm::vec3 ScaleAmount(2, 2, 2);
	// cria a matriz de escala
	glm::mat4 Scale = glm::scale(I, ScaleAmount);
	// imprime
	PrintMatrix(Scale);
	// cria o vetor de posição
	glm::vec4 Position(100, 100, 0, 1);
	// cria o vetor de direção
	glm::vec4 Direction(100, 100, 0, 0);
	
	// escala o vetor de posição
	Position = Scale * Position;
	// escala o vetor de direção
	Direction = Scale * Direction; // tem magnitude
	// imprime
	std::cout << glm::to_string(Position) << std::endl;
	// o vetor de direção também sofre alteração de 
	// escala
	std::cout << glm::to_string(Direction) << std::endl;
}

void RotationMatrix() {
	// pula linha
	std::cout << std::endl;
	// mensagem
	std::cout << "Rotation Matrix" << std::endl;
	// matriz identidade
	glm::mat4 I = glm::identity<glm::mat4>();
	// ângulo de rotação
	// 1.57 radianos
	constexpr float Angle = glm::radians(90.0f); // graus para radianos
	glm::vec3 Axis{ 0,0,1 }; // eixo de rotação z
	// cria a matriz de rotação
	glm::mat4 Rotation = glm::rotate(I, Angle, Axis);

	// cria o vetor de posição
	glm::vec4 Position{ 100, 0, 0, 1 };
	glm::vec4 Direction{ 100, 0, 0, 0 };

	// rotação
	Position = Rotation * Position;
	Direction = Rotation * Direction;

	// a rotação altera a posição
	std::cout << glm::to_string(Position) << std::endl;
	// a rotação altera a direção
	std::cout << glm::to_string(Direction) << std::endl;
}

int main() {
	
	std::cout << "Olá matrizes" << std::endl;
	//TranslationMatrix();
	//ScaleMatrix();
	RotationMatrix();
}