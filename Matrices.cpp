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
	PrintMatrix(Rotation);
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


void ComposedMatrices() {
	// pula linha
	std::cout << std::endl;
	// mensagem
	std::cout << "Composed Matrix" << std::endl;
	// matriz de identidade
	glm::mat4 I = glm::identity<glm::mat4>();
	// matriz de translação
	glm::vec3 T{ 0, 10, 0 }; // desloca 10 em y
	glm::mat4 TranslationMatrix = glm::translate(I, T);
	std::cout << "Translation Matrix" << std::endl;
	PrintMatrix(TranslationMatrix);
	// matriz de rotação
	// converte angulos em radianos - 0.79 radianos
	constexpr float Angle = glm::radians(90.0f); // ângulo de rotação 45
	glm::vec3 Axis{ 0,0,1 }; // eixo de rotação
	glm::mat4 RotationMatrix = glm::rotate(I, Angle, Axis);
	std::cout << "Rotation Matrix" << std::endl;
	PrintMatrix(RotationMatrix);
	// matriz de escala
	glm::vec3 ScaleAmount(2, 2, 0); // dobra largura e dobra altura
	glm::mat4 ScaleMatrix = glm::scale(I, ScaleAmount);
	std::cout << "Scale Matrix" << std::endl;
	PrintMatrix(ScaleMatrix);
	// cria um vetor de posição
	glm::vec4 Position(1, 1, 0, 1);
	// cria um vetor de direção
	glm::vec4 Direction(1, 1, 0, 0);

	// primeiro escala, segundo rotaciona e depois translada
	// multiplicação de matrizes não é comutativa, a ordem importa
	// opera da direita para a esquerda
	glm::mat4 ModelMatrix =
	TranslationMatrix * RotationMatrix * ScaleMatrix;
	std::cout << "Model Matriz" << std::endl;
	PrintMatrix(ModelMatrix);

	// não devemos fazer assim - muito processamento
	Position = ScaleMatrix * Position;
	std::cout << glm::to_string(Position) << std::endl;
	Position = RotationMatrix * Position;
	std::cout << glm::to_string(Position) << std::endl;
	Position = TranslationMatrix * Position;
	std::cout << glm::to_string(Position) << std::endl;

	// aplicando a composição no vetor de posição
	//Position = ModelMatrix * Position;
	//std::cout << glm::to_string(Position) << std::endl;
	//Direction = ModelMatrix * Direction;
	///std::cout << glm::to_string(Direction) << std::endl;
}

void ModelViewProjection()
{
	std::cout << std::endl;
	std::cout << "==========================" << std::endl;
	std::cout << "Model, View and Projection" << std::endl;
	std::cout << "==========================" << std::endl;

	// Model é a matriz formada pelas transformações de 
	// Escala, Rotação e Translação!
	glm::mat4 Model = glm::identity<glm::mat4>();

	// Matrix View - onde está a câmera para ver o modelo
	// onde a câmera virtual está
	glm::vec3 Eye{ 0.0f, 0.0f, 10.0f }; // posição
	glm::vec3 Center{ 0.0f, 0.0f, 0.0 }; // direção
	// 0.0f, -1.0f, 0.0f // vendo de cabeça pra baixo
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f }; // orientação
	glm::mat4 View = glm::lookAt(Eye, Center, Up);

	std::cout << "View: " << std::endl;
	PrintMatrix(View);

	// angulo de visão - 45o.
	constexpr float FoV = glm::radians(45.0f);
	// razão de aspecto
	const float AspectRatio = 800.0f / 600.0f;
	// distância do plano perto da câmera
	const float Near = 0.001f;
	// distância do plano longe da câmera
	const float Far = 1000.0f;
	glm::mat4 Projection = glm::perspective(FoV, AspectRatio, Near, Far);

	std::cout << "Projection: " << std::endl;
	PrintMatrix(Projection);

	glm::mat4 ModelViewProjection = Projection * View * Model;

	std::cout << "ModelViewProjection: " << std::endl;
	PrintMatrix(ModelViewProjection);

	glm::vec4 Position{ 0, 0, 0, 1 };
	Position = ModelViewProjection * Position;
	std::cout << glm::to_string(Position) << std::endl;
	Position = Position / Position.w;
	std::cout << glm::to_string(Position) << std::endl;
}

int main() {
	
	std::cout << "Olá matrizes" << std::endl;
	//TranslationMatrix();
	//ScaleMatrix();
	//RotationMatrix();
	//ComposedMatrices();
	ModelViewProjection();
}