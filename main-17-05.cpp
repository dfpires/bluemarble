
#include <array>

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp> // glm::identity glm::loolAt 

#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // biblioteca para carregar imagens

const int Width = 800;
const int Height = 600;

// le o conteúdo de um arquivo
std::string ReadFile(const char* FilePath) {
	// cria variável para guardar conteúdo do arquivo
	std::string FileContents;
	// abrir um arquivo FilePath para leitura in - input arquivo de entrada
	if (std::ifstream FileStream{ FilePath, std::ios::in }) {
		// percorre o conteúdo do arquivo e copia para FileContents
		FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return FileContents; // retorna o conteúdo do arquivo

}

// PODEMOS RECEBER VERTEXSHADERID OU FRAGMENTSHARDERID
void CheckShader(GLuint ShaderId) {
	// VARIÁVEL VAI GUARDAR TRUE SE NÃO TEM ERRO E FALSE SE TEM ERRO
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
	// ler o conteúdo do arquivo VertexShader
	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	// ler o conteúdo do arquivo FragmentShader
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);
	// verifica se o arquivo VertexShader não está vazio
	assert(!VertexShaderSource.empty()); // assert(false) sai do programa
	// verifica se o arquivo FragmentShader não está vazio
	assert(!FragmentShaderSource.empty()); // assert(false) sai do programa

	// cada shader precisa ter um Id para ser identificado
	// criar id para o VertexShader
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// criar id para o FragmentShader
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// cria um ponteiro para o conteúdo do arquivo VertexShader
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	// o Id do VertexShader vai referenciar o conteúdo do arquivo - 1 é um shader
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	// vamos compilar o Vertex em tempo de execução
	glCompileShader(VertexShaderId);
	// VAMOS CHECAR SE NÃO HOUVE ERRO NO SHADER
	CheckShader(VertexShaderId);

	// cria um ponteiro para o conteúdo do arquivo FragmentShader
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	// o Id do FragmentShader vai referenciar o conteúdo do arquivo - 1 é um shader
	glShaderSource(FragmentShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	// vamos compilar o Fragment em tempo de execução
	glCompileShader(FragmentShaderId);
	// VAMOS CHECAR SE NÃO HOUVE ERRO NO SHADER
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

	glDeleteShader(VertexShaderId);  // remove o VertexShader da memória
	glDeleteShader(FragmentShaderId); // remove o FragmentShader da memória

	return ProgramId; // retorna o programa conteúdo o Vertex e o Fragment Shaders

}

// criamos um tipo de dados contendo um vetor de posição e um vetor de cores
struct Vertex {
	glm::vec3 Position; // layout 0
	glm::vec3 Color; // layout 1
	glm::vec2 UV;// layout 2	
};


GLuint LoadGeometry() {

}
// criando uma classe chamada SimpleCamera
class SimpleCamera
{
public: // variáveis e os métodos serão públicis


	void MoveForward(float Amount)
	{
		// localização da câmera depende da direção
		Location += Direction * Amount * Speed;
	}

	void MoveRight(float Amount)
	{
		// localização da câmera depende do vetor right, que por sua vez depende
		// da direção e da orientação
		glm::vec3 Right = glm::cross(Direction, Up);
		Location += Right * Amount;
	}

	// obtem a matriz de projeção e de visão
	glm::mat4 GetViewProjection()
	{
		glm::mat4 View = glm::lookAt(Location, Location + Direction, Up);
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * View;
	}


	void Look(float Yaw, float Pitch)
	{
		Yaw *= Sensitivity;
		Pitch *= Sensitivity;
		// criando o vetor Right a partir de Direction e do Up
		const glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
		// criando a matriz identidade
		const glm::mat4 I = glm::identity<glm::mat4>();

		// criar a rotação Yaw - em y , por isso o Up está envolvido
		glm::mat4 YawRotation = glm::rotate(I, glm::radians(Yaw), Up);
		// criar a rotação Pitch - em x, por isso o Right está envolvido
		glm::mat4 PitchRotation = glm::rotate(I, glm::radians(Pitch), Right);

		// altera ou atualiza o vetor Up
		Up = PitchRotation * glm::vec4{ Up, 0.0f };
		// altera ou atualiza o vetor Direction 
		Direction = YawRotation * PitchRotation * glm::vec4{ Direction, 0.0f };
	}

	// localização da câmera
	glm::vec3 Location = { 0.0f, 0.0f, 10.0f };
	// direção da câmera
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	// orientação da câmera
	glm::vec3 Up = { 0.0f, 1.0f, 0.0f };

	// velocidade de movimentação
	float Speed = 5.0f;
	float Sensitivity = 0.1f;

	// campo de visão
	float FieldOfView = glm::radians(45.0f);
	// razão de aspecto
	float AspectRatio = Width / Height;
	// posição do campo mais perto
	float Near = 0.01f;
	// posição do campo mais longe
	float Far = 1000.0f;

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
	// número de componentes
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

	// Copia a textura para a memória da GPU
	GLint Level = 0;
	GLint Border = 0;
	glTexImage2D(GL_TEXTURE_2D, Level, GL_RGB, TextureWidth, TextureHeight, Border, 
		GL_RGB, GL_UNSIGNED_BYTE, TextureData);

	// filtro de magnificação - mais suave quando está fazendo ZOOM na imagem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// filtro de mimificação - para não ter problema de aliasing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// caso não cubra toda a imagem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// gerar o mipmap a partir da textura
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); // desabilita a textura

	stbi_image_free(TextureData); // libera a memória

	return TextureId;
}

// cria um objeto da classe SimpleCamera
SimpleCamera Camera;
bool bEnableMouseMovement = false;
glm::vec2 PreviousCursor;

// função executada automaticamente quando o botão do mouse for clicado
void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers)
{
	 std::cout << "Button: " << Button << " Action: " << Action << " Modifiers: " << Modifiers << std::endl;

	if (Button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (Action == GLFW_PRESS)
		{
			// desabilita o cursor do mouse
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			// obter as posição X e Y do mouse
			glfwGetCursorPos(Window, &X, &Y);
			// atualizo a posição do cursor anterior
			PreviousCursor = glm::vec2{ X, Y };
			// habilita movimento do mouse
			bEnableMouseMovement = true;
		}
		else if (Action == GLFW_RELEASE)
		{
			// habilita cursor do mouse
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// desabilita movimento do mouse
			bEnableMouseMovement = false;
		}
	}
}

// função executada automaticamente quando o mouse for movimentado
void MouseMotionCallback(GLFWwindow* Window, double X, double Y)
{
	if (bEnableMouseMovement)
	{	
		// posição corrente do cursos
		glm::vec2 CurrentCursor{ X, Y };
		// posição anterior - posição atual
		glm::vec2 Delta = PreviousCursor - CurrentCursor;

		 std::cout << glm::to_string(Delta) << std::endl;
		// atualiza a câmera - matriz de visão
		Camera.Look(-Delta.x, -Delta.y);
		// posição anterior será a posição corrente
		PreviousCursor = CurrentCursor;
	}
}

void ResizeCallback(GLFWwindow* Window, int Width, int Height)
{
	Camera.AspectRatio = static_cast<float>(Width) / Height;
	glViewport(0, 0, Width, Height);
}



int main() {

	glfwInit();

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	

	glfwSetFramebufferSizeCallback(Window, ResizeCallback);

	// registra a função de callback MouseButtonCallback
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, MouseMotionCallback);

	glfwMakeContextCurrent(Window);

	// permite que os fps sejam alterados, para mais lento ou mais rápido
	// não ultrapassa o limite da placa de vídeo
	glfwSwapInterval(0);

	glewInit();

	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGL Version  : " << GLMajorVersion << "." << GLMinorVersion << std::endl;

	// Usar o glGetString() para obter informações sobre o driver que estamos usando
	std::cout << "OpenGL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// chama a função ReadFile para ler o conteúdo dos arquivos shaders
	std::string VertexShaderSource = ReadFile("shaders/triangle_vert.glsl");
	std::cout << VertexShaderSource << std::endl;
	std::string FragmentShaderSource = ReadFile("shaders/triangle_frag.glsl");
	std::cout << FragmentShaderSource << std::endl;

	// cria um programa composto de vertex shader e do fragment shader
	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	

	// define um triângulo em coordenadas normalizadas
	// array do tipo Vertex, onde cada elemento do array será um Vertex, que contém
	// um vetor de posição e um vetor de cores
	// a variável mudou de nome, para Quad -> quadrado
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
		Vertex { glm::vec3{ 1.0f, 1.0f, 0.0f},
				 glm::vec3{1.0f, 0.0f, 0.0f},
				 glm::vec2{1.0f, 1.0f}},

		// IV
		Vertex { glm::vec3{ -1.0f, 1.0f, 0.0f},
				 glm::vec3{0.0f, 0.0f, 1.0f},
			     glm::vec2{0.0f, 1.0f}},

	};
	
	// vamos definir o índice
	std::array<glm::ivec3, 2> Indices = {
		glm::ivec3{0, 1, 3},
		glm::ivec3{3, 1, 2}
	};

	// Model View Projection
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	// vamos copiar os vértices do triangulo para a memória da GPU
	GLuint VertexBuffer;

	// gerar o identificador
	glGenBuffers(1, &VertexBuffer);

	// Pedir para a OpenGL gerar identificador do EBO
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);


	// ativa glBufferVertex para receber a cópia
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// ativa ElementBuffer para receber a cópia
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

	// copia os dados para a memória de vídeo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	// Carregar a Textura para a Memória de Vídeo
	GLuint TextureId = LoadTexture("textures/earth_2k.jpg");
	
	// Define a cor de fundo
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);

	// é em termos de fps
	// é o tempo que houve a última atualização de frame
	double PreviousTime = glfwGetTime();

	while (!glfwWindowShouldClose(Window)) {
		// é o tempo que ocorreu a atualização de frame atual
		double CurrentTime = glfwGetTime();
		double DeltaTime = CurrentTime - PreviousTime;
		if (DeltaTime > 0.0)
		{
			// Camera.Update(static_cast<float>(DeltaTime));
			PreviousTime = CurrentTime;
		}
		// glClear vai limpar o framebuffer. GL_COLOR_BUFFER_BIT diz para limpar o buffer de
// cor. Depois de limpar ele vai preencher com a cor que foi configurada em glClearColor
// Quando formos desenhar geometrias 3D, vamos voltar ao glClear porque teremos que limpar
// o buffer de profundidade (depth buffer)
		glClear(GL_COLOR_BUFFER_BIT);

		// ativar o programa do shader
		glUseProgram(ProgramId);

		glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection();

		glm::mat4 ModelViewProjection = ViewProjectionMatrix * ModelMatrix;
		// processamento sendo executado na GPU
		// obtemos a localização do uniforme ModelViewProjection
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
	
			// o 0 indica o layout de posição
		glEnableVertexAttribArray(0);
		// o 1 indica o layou de cores
		glEnableVertexAttribArray(1);
		// o 2 indica coordenada de textura
		glEnableVertexAttribArray(2);

		// informa ao OpenGL que VertexBuffer será o ativo no momento
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		// informa ao OpenGL que ElementArrayBuffer será o ativo no momento
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

		// informa ao OpenGL onde, dentro do vertex buffer, os vértices estão
		// array é contíguoa em memória, basta dizer quantos vértices serão utilizados
		// sizeof(Vertex) é o stride - 24
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		// offsetof(Vertex) é o offset - 12
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), 
			reinterpret_cast<void*> (offsetof(Vertex, Color)));

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
			reinterpret_cast<void*> (offsetof(Vertex, UV)));


		// finalmente dispara o OpenGL e desenha triangulo no vertex buffer
		//glDrawArrays(GL_TRIANGLES, 0, Quad.size());
		// glPointSize(10.0f);
		//glDrawArrays(GL_POINTS, 0, Quad.size());
		//  glLineWidth(10.0f);
		//glDrawArrays(GL_LINES, 0, Quad.size());
		// glDrawArrays(GL_LINE_STRIP, 0, Quad.size());
		//glDrawArrays(GL_LINE_LOOP, 0, Quad.size());
		 //glDrawArrays(GL_TRIANGLE_STRIP, 0, Quad.size());
		// glDrawArrays(GL_TRIANGLE_FAN, 0, Quad.size());
		// 
		glPointSize(10.0f);
		 glLineWidth(10.0f);
		 // glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
		  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		 //glDrawArrays(GL_TRIANGLES, 0, Quad.size());
		 glDrawElements(GL_TRIANGLES, Indices.size() * 3, GL_UNSIGNED_INT, nullptr);
	
		 // reverte o estado criado
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glUseProgram(0);
		// processo todos os eventos da fila de eventos do GLFW
		// eventos: teclado, mouse, gamepad
		glfwPollEvents();
		// envia o conteúdo do framebuffer da janela para ser desenhado na tela
		glfwSwapBuffers(Window);

		// tratamento de evento com teclado
		// tecla digitada foi o W -> ir pra frente
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Camera.MoveForward(1.0f * DeltaTime);
		}
		// move-se para trás
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Camera.MoveForward(-1.0f * DeltaTime);
		}
		// move-se para esquerda
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Camera.MoveRight(-1.0 * DeltaTime);
		}
		// move-se para direita
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Camera.MoveRight(1.0f * DeltaTime);
		}
	}

	// desaloca 
	glDeleteBuffers(1, &VertexBuffer);

	glfwTerminate();

	return 0;
}

