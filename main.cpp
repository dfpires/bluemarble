
#include <iostream>
#include <glm/glm.hpp>

#include <glm/gtx/string_cast.hpp>

int main() {

	glm::vec3 V(1, 2, 3);

	// definindo a saída padrão
	std::cout << "Boa noite, já sabemos programar em C++" << std::endl;
	std::cout << glm::to_string(V) << std::endl;
	return 0;
}
