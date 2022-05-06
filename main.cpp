
#include <array>

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp> // glm::identity glm::loolAt 

#include <GLFW/glfw3.h>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // biblioteca para carregar imagens

const int Width = 800;
const int Height = 600;

// le o conte�do de um arquivo
std::string ReadFile(const char* FilePath) {
	// cria vari�vel para guardar conte�do do arquivo
	std::string FileContents;
	// abrir um arquivo FilePath para leitura in - input arquivo de entrada
	if (std::ifstream FileStream{ FilePath, std::ios::in }) {
		// percorre o conte�do do arquivo e copia para FileContents
		FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return FileContents; // retorna o conte�do do arquivo

}

// PODEMOS RECEBER VERTEXSHADERID OU FRAGMENTSHARDERID
void CheckShader(GLuint ShaderId) {
	// VARI�VEL VAI GUARDAR TRUE SE N�O TEM ERRO E FALSE SE TEM ERRO
	GLint Result = GL_TRUE;
	// CHECA STATUS E RETORNA EM RESULT
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE) { // TEMOS ERRO
		// houve um erro ao compilar o shader
		GLint InfoLogLength = 0; // GUARDA O TAMANHO DO ERRO
		// OBTER O TAMANHO DO ERRO E ATRIBUO PARA INFOLOGLENGTH
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		std::string ShaderInfoLog(InfoLogLength, '\0');
		glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);
		
		std::cout << " Erro no shader " << std::endl;
		std::cout << ShaderInfoLog << std::endl;

		assert(false); // ENCERRA O PROGRAMA
	}
}
// carrega os shaders e retorna um programa
GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile) {
	// ler o conte�do do arquivo VertexShader
	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	// ler o conte�do do arquivo FragmentShader
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);
	// verifica se o arquivo VertexShader n�o est� vazio
	assert(!VertexShaderSource.empty()); // assert(false) sai do programa
	// verifica se o arquivo FragmentShader n�o est� vazio
	assert(!FragmentShaderSource.empty()); // assert(false) sai do programa

	// cada shader precisa ter um Id para ser identificado
	// criar id para o VertexShader
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// criar id para o FragmentShader
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// cria um ponteiro para o conte�do do arquivo VertexShader
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	// o Id do VertexShader vai referenciar o conte�do do arquivo - 1 � um shader
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	// vamos compilar o Vertex em tempo de execu��o
	glCompileShader(VertexShaderId);
	// VAMOS CHECAR SE N�O HOUVE ERRO NO SHADER
	CheckShader(VertexShaderId);

	// cria um ponteiro para o conte�do do arquivo FragmentShader
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	// o Id do FragmentShader vai referenciar o conte�do do arquivo - 1 � um shader
	glShaderSource(FragmentShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	// vamos compilar o Fragment em tempo de execu��o
	glCompileShader(FragmentShaderId);
	// VAMOS CHECAR SE N�O HOUVE ERRO NO SHADER
	CheckShader(FragmentShaderId);

	// vamos criar o programa
	GLuint ProgramId = glCreateProgram();
	// o programa deve ter um VertexShader representado por seu Id
	glAttachShader(ProgramId, VertexShaderId);
	// o programa deve ter um FragmentShader representado por seu Id
	glAttachShader(ProgramId, FragmentShaderId);
	// vamos montar o programa
	glLinkProgram(ProgramId);

	// vamos verificar se o programa foi montado com sucesso
	GLint Result = GL_TRUE;
	// vai me retornar o status do programa e joga o resultado em Result
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
	// se o Result estiver falso
	if (Result == GL_FALSE) {

		GLint InfoLogLength = 0; // GUARDA O TAMANHO DA MENSAGEM DE ERRO
		// ARMAZENAR O TAMANHO EM INFOLOGLENGTH
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) { // TEMOS UM ERRO
			std::string ProgramInfoLog(GL_INFO_LOG_LENGTH, '\0');
			glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::cout << "Erro ao linkar o programa " << std::endl;
			std::cout << ProgramInfoLog << std::endl;

			assert(false); // SAI DO PROGRAMA
		}
		
	}

	glDetachShader(ProgramId, VertexShaderId); // separa o VertexShader do programa
	glDetachShader(ProgramId, FragmentShaderId); // separa o FragmentShader do programa

	glDeleteShader(VertexShaderId);  // remove o VertexShader da mem�ria
	glDeleteShader(FragmentShaderId); // remove o FragmentShader da mem�ria

	return ProgramId; // retorna o programa conte�do o Vertex e o Fragment Shaders

}

// criamos um tipo de dados contendo um vetor de posi��o e um vetor de cores
struct Vertex {
	glm::vec3 Position; // layout 0
	glm::vec3 Color; // layout 1
	glm::vec2 UV;// layout 2	
};

// carrega o arquivo contendo a textura
GLuint LoadTexture(const char* TextureFile)
{
	std::cout << "Carregando Textura " << TextureFile << std::endl;

	// giro na vertical
	stbi_set_flip_vertically_on_load(true);
	// largura da textura
	int TextureWidth = 0;
	// altura da textura
	int TextureHeight = 0;
	// n�mero de componentes
	int NumberOfComponents = 0;

	// stbi -> biblioteca em C++ para carregar arquivos do tipo imagem
	unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, 
		&NumberOfComponents, 3);

	// verifica se deu erro, caso sim, saiu do programa
	assert(TextureData);

	// Gerar o Identifador da Textura para enviar para a GPU
	GLuint TextureId;
	glGenTextures(1, &TextureId);

	// Habilita a textura para ser modificada
	glBindTexture(GL_TEXTURE_2D, TextureId);

	// Copia a textura para a mem�ria da GPU
	GLint Level = 0;
	GLint Border = 0;
	glTexImage2D(GL_TEXTURE_2D, Level, GL_RGB, TextureWidth, TextureHeight, Border, 
		GL_RGB, GL_UNSIGNED_BYTE, TextureData);

	// filtro de magnifica��o - mais suave quando est� fazendo ZOOM na imagem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// filtro de mimifica��o - para n�o ter problema de aliasing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// caso n�o cubra toda a imagem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// gerar o mipmap a partir da textura
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); // desabilita a textura

	stbi_image_free(TextureData); // libera a mem�ria

	return TextureId;
}


int main() {

	glfwInit();

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	
	glfwMakeContextCurrent(Window);

	glewInit();

	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGL Version  : " << GLMajorVersion << "." << GLMinorVersion << std::endl;

	// Usar o glGetString() para obter informa��es sobre o driver que estamos usando
	std::cout << "OpenGL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// chama a fun��o ReadFile para ler o conte�do dos arquivos shaders
	std::string VertexShaderSource = ReadFile("shaders/triangle_vert.glsl");
	std::cout << VertexShaderSource << std::endl;
	std::string FragmentShaderSource = ReadFile("shaders/triangle_frag.glsl");
	std::cout << FragmentShaderSource << std::endl;

	// cria um programa composto de vertex shader e do fragment shader
	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	

	// define um tri�ngulo em coordenadas normalizadas
	// array do tipo Vertex, onde cada elemento do array ser� um Vertex, que cont�m
	// um vetor de posi��o e um vetor de cores
	// a vari�vel mudou de nome, para Quad -> quadrado
	// 3 foi alterado para 6
	std::array<Vertex, 6> Quad = {
		// I
		Vertex { glm::vec3{-1.0f, -1.0f, 0.0f}, 
				 glm::vec3{1.0f, 0.0f, 0.0f}, 
				 glm::vec2{0.0f, 0.0f}},
		// II
		Vertex { glm::vec3{ 1.0f, -1.0f, 0.0f}, 
				 glm::vec3{0.0f, 1.0f, 0.0f}, 
				 glm::vec2{1.0f, 0.0f}},
		
		// III 
		Vertex { glm::vec3{ -1.0f,  1.0f, 0.0f}, 
				 glm::vec3{0.0f, 0.0f, 1.0f}, 
				 glm::vec2{0.0f, 1.0f}},
		// III
		Vertex { glm::vec3{ -1.0f,  1.0f, 0.0f},
				 glm::vec3{0.0f, 0.0f, 1.0f},
				 glm::vec2{0.0f, 1.0f}},
		// II
		Vertex { glm::vec3{ 1.0f, -1.0f, 0.0f},
				 glm::vec3{0.0f, 1.0f, 0.0f},
				 glm::vec2{1.0f, 0.0f}},
		// IV
		Vertex { glm::vec3{ 1.0f, 1.0f, 0.0f},
				 glm::vec3{1.0f, 0.0f, 0.0f},
				 glm::vec2{1.0f, 1.0f}},
	};
	
	// Model View Projection
	glm::mat4 Model = glm::identity<glm::mat4>();

	// View (C�mera)
	glm::vec3 Eye{ 0.0f, 0.0f, 10.0f }; // posi��o 
	glm::vec3 Center{ 0.0f, 0.0f, 0.0 }; // dire��o
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f }; // orienta��o
	// define a c�mera
	glm::mat4 View = glm::lookAt(Eye, Center, Up);

	// Projection
	constexpr float FoV = glm::radians(20.0f); // campo de vis�o
	const float AspectRatio = Width / Height; // raz�o de aspecto
	const float Near = 0.001f; // dist�ncia perto
	const float Far = 1000.0f; // dist�ncia longo
	// define a proje��o
	glm::mat4 Projection = glm::perspective(FoV, AspectRatio, Near, Far);

	// ModelViewProjection
	glm::mat4 ModelViewProjection = Projection * View * Model;

	/* // Aplicar a ModelViewProjection nos v�rtices do tri�ngulo
	// para cada Vertex do array -> CPU
	//for (Vertex& Vertex : Triangle) {
		// precisamos tranformar vec3 em vec4
//		glm::vec4 ProjectedVertex = 
			ModelViewProjection *  glm::vec4{ Vertex.Position, 1.0f };
		// transformando o ProjectedVertex em coordenadas normalizada - 0 e 1
		// dividir x, y e z por w garante que as coordenadas estejam entre 0 e 1
		ProjectedVertex /= ProjectedVertex.w;
		// colocamos a altera��o no Vertex
		Vertex.Position = ProjectedVertex;
	}
	*/

	// vamos copiar os v�rtices do triangulo para a mem�ria da GPU
	GLuint VertexBuffer;

	// gerar o identificador
	glGenBuffers(1, &VertexBuffer);

	// ativa glBufferVertex para receber a c�pia
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);


	// copia os dados para a mem�ria de v�deo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

	// Carregar a Textura para a Mem�ria de V�deo
	GLuint TextureId = LoadTexture("textures/earth_2k.jpg");
	
	// Define a cor de fundo
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);

	while (!glfwWindowShouldClose(Window)) {
		// glClear vai limpar o framebuffer. GL_COLOR_BUFFER_BIT diz para limpar o buffer de
// cor. Depois de limpar ele vai preencher com a cor que foi configurada em glClearColor
// Quando formos desenhar geometrias 3D, vamos voltar ao glClear porque teremos que limpar
// o buffer de profundidade (depth buffer)
		glClear(GL_COLOR_BUFFER_BIT);

		// ativar o programa do shader
		glUseProgram(ProgramId);

		// processamento sendo executado na GPU
		// obtemos a localiza��o do uniforme ModelViewProjection
		GLint ModelViewProjectionLoc = 
			glGetUniformLocation(ProgramId, "ModelViewProjection");
		// insere o uniforme na GPU
		glUniformMatrix4fv(ModelViewProjectionLoc, 
			1, 
			GL_FALSE, 
			glm::value_ptr(ModelViewProjection));

		// ativa a TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);
	
			// o 0 indica o layout de posi��o
		glEnableVertexAttribArray(0);
		// o 1 indica o layou de cores
		glEnableVertexAttribArray(1);
		// o 2 indica coordenada de textura
		glEnableVertexAttribArray(2);

		// informa ao OpenGL que VertexBuffer ser� o ativo no momento
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		// informa ao OpenGL onde, dentro do vertex buffer, os v�rtices est�o
		// array � cont�guoa em mem�ria, basta dizer quantos v�rtices ser�o utilizados
		// sizeof(Vertex) � o stride - 24
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		// offsetof(Vertex) � o offset - 12
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), 
			reinterpret_cast<void*> (offsetof(Vertex, Color)));

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
			reinterpret_cast<void*> (offsetof(Vertex, UV)));


		// finalmente dispara o OpenGL e desenha triangulo no vertex buffer
		glDrawArrays(GL_TRIANGLES, 0, Quad.size());

		// reverte o estado criado
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glUseProgram(0);
		// processo todos os eventos da fila de eventos do GLFW
		// eventos: teclado, mouse, gamepad
		glfwPollEvents();
		// envia o conte�do do framebuffer da janela para ser desenhado na tela
		glfwSwapBuffers(Window);
	}

	// desaloca 
	glDeleteBuffers(1, &VertexBuffer);

	glfwTerminate();

	return 0;
}

