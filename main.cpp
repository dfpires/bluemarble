
#include <iostream>
#include <glm/glm.hpp>

#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

const int width = 800; // largura da janela será de 800 pixels
const int heigth = 600; // altura da janela será de 600 pixes


int main() {

	glm::vec3 V(1, 2, 3);

	// definindo a saída padrão
	std::cout << "Boa noite, já sabemos programar em C++" << std::endl;
	std::cout << glm::to_string(V) << std::endl;

	glfwInit(); // inicializa a glfw

	// referência para a janela
	GLFWwindow* window = glfwCreateWindow(width, heigth, "BlueMarble", nullptr, nullptr);

	// enquanto a janela não for fechada
	while (!glfwWindowShouldClose(window)) {
		// processar todos os eventos da fila de eventos do GLFW (teclado, mouse, gamepad)
		glfwPollEvents();
		// envia o conteúdo do framebuffer da janela para ser desenhado na tela
		glfwSwapBuffers(window);
	}

	glfwTerminate(); // encerra a janela

	glewInit();

	return 0;
}
