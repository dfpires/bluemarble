
#include <array>

#include <iostream>

#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp> // glm::identity glm::loolAt 

#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/gtx/string_cast.hpp>

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
	glm::vec3 Normal; // normal - ilumina��o
	glm::vec3 Color; // layout 1
	glm::vec2 UV;// layout 2	
};


struct DirectionalLight
{
	glm::vec3 Direction;
	GLfloat Intensity;
};

GLuint LoadGeometry() {

	// define um tri�ngulo em coordenadas normalizadas
	// array do tipo Vertex, onde cada elemento do array ser� um Vertex, que cont�m
	// um vetor de posi��o e um vetor de cores
	// a vari�vel mudou de nome, para Quad -> quadrado
	// 3 foi alterado para 6
	std::array<Vertex, 6> Quad = {
		// I
		Vertex { glm::vec3{-1.0f, -1.0f, 0.0f},
				glm::vec3(0.0f, 0.0f, 1.0f),// -ilumina��o
				 glm::vec3{1.0f, 0.0f, 0.0f},
				 glm::vec2{0.0f, 0.0f}},
		// II
		Vertex { glm::vec3{ 1.0f, -1.0f, 0.0f},
				glm::vec3(0.0f, 0.0f, 1.0f), // -ilumina��o
				glm::vec3{0.0f, 1.0f, 0.0f},
				glm::vec2{1.0f, 0.0f}},

		 // III
		Vertex { glm::vec3{ 1.0f, 1.0f, 0.0f},
				glm::vec3(0.0f, 0.0f, 1.0f), // -ilumina��o
				glm::vec3{1.0f, 0.0f, 0.0f},
				 glm::vec2{1.0f, 1.0f}},

		// IV
		Vertex { glm::vec3{ -1.0f, 1.0f, 0.0f},
				glm::vec3(0.0f, 0.0f, 1.0f), // -ilumina��o
				glm::vec3{0.0f, 0.0f, 1.0f},
				glm::vec2{0.0f, 1.0f}},

	};

	// vamos definir o �ndice
	std::array<glm::ivec3, 2> Indices = {
		glm::ivec3{0, 1, 3},
		glm::ivec3{3, 1, 2}
	};

	// vamos copiar os v�rtices do triangulo para a mem�ria da GPU
	GLuint VertexBuffer;

	// gerar o identificador
	glGenBuffers(1, &VertexBuffer);

	// Pedir para a OpenGL gerar identificador do EBO
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);


	// ativa glBufferVertex para receber a c�pia
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// ativa ElementBuffer para receber a c�pia
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

	// copia os dados para a mem�ria de v�deo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);


	// gerar o VertexArrayObject - VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// habilita o VAO
	glBindVertexArray(VAO);

	// o 0 indica o layout de posi��o
	glEnableVertexAttribArray(0);
	// o 1 indica o vetor normal
	glEnableVertexAttribArray(1); // -ilumina��o
	// o 2 indica o layou de cores
	glEnableVertexAttribArray(2);
	// o 3 indica coordenada de textura
	glEnableVertexAttribArray(3);

	// informa ao OpenGL que VertexBuffer ser� o ativo no momento
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	// informa ao OpenGL onde, dentro do vertex buffer, os v�rtices est�o
	// array � cont�guoa em mem�ria, basta dizer quantos v�rtices ser�o utilizados
	// sizeof(Vertex) � o stride - 24
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	// offsetof(Vertex) � o offset - 12
	// -ilumina��o
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, Normal)));

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, Color)));

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, UV)));

	// encerra o estado de copiar conte�do para o GPU
	glBindVertexArray(0);

	return VAO;
}

// fun��o que vai gerar os pontos da esfera
// os pontos s�o inseridos no vetor de Vertex (posi��es, cores, coordenadas de
// textura)
// fun��o que vai gerar os tri�ngulos da esfera
// vamos criar os pontos dos tri�ngulos, assim como os �ndice, no Indices
void GenerateSphereMesh(GLuint Resolution, std::vector<Vertex>& Vertices,
	std::vector<glm::ivec3>& Indices) {

	// garante que o vetor estar� vazio / limpo
	Vertices.clear();
	Indices.clear();

	// Pi representa o valor de Pi 3.141516 .......
	constexpr float Pi = glm::pi<float>();
	// TwoPi representa 2 Pis
	constexpr float TwoPi = glm::two_pi<float>();
	// inverso da resolu��o -> pois podemos apenas multiplicar
	float InvResolution = 1.0f / static_cast<float>(Resolution - 1);


	for (GLuint UIndex = 0; UIndex < Resolution; ++UIndex) {

		// U � o valor de x da coordenada de textura
		// U � obtido a partir do UIndex / resolution
		const float U = UIndex * InvResolution; // coordenada normalizada
		// calcular o teta
		const float Theta = glm::mix(0.0f, Pi, U); // interpola��o


		for (GLuint VIndex = 0; VIndex < Resolution; ++VIndex) {
			// V � o valor de y da coordenada de textura
			// V � obtido a partir do VIndex / resolution
			const float V = VIndex * InvResolution;
			// calcular o phi
			const float Phi = glm::mix(0.0f, TwoPi, V); // interpola��o

			glm::vec3 VertexPosition = {
				glm::sin(Theta) * glm::cos(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Theta)
			};
			// criar um Vertex
			Vertex Vertex{
				VertexPosition,
				glm::normalize(VertexPosition), // -ilumina��o
				glm::vec3{1.0f, 1.0f, 1.0f},
				glm::vec2{1.0f - U, V}
			};
			// adiciona Vertex em Vertices
			Vertices.push_back(Vertex);
		}
	}

	// para U de 0 at� resolu��o -1
	for (GLuint U = 0; U < Resolution - 1; ++U) {
		// para V de 0 at� resolu��o -1	
		for (GLuint V = 0; V < Resolution - 1; ++V) {
			GLuint P0 = U + V * Resolution;
			GLuint P1 = (U + 1) + V * Resolution;
			GLuint P2 = (U + 1) + (V + 1) * Resolution;
			GLuint P3 = U + (V + 1) * Resolution;
			// criando o vetor conte�do os �ndices dos tri�ngulos
			Indices.push_back(glm::ivec3{ P0, P1, P3 });
			Indices.push_back(glm::ivec3{ P3, P1, P2 });
		}
	}
}

// carrega a esfera no buffer da placa de v�deo
// retorna o VAO
// & -> representa o endere�o de mem�ria da vari�vel
GLuint LoadSphere(GLuint& numVertices, GLuint& numIndices) {

	std::vector<Vertex> Vertices;
	std::vector<glm::ivec3> Triangles; // vetor de �ndices

	// chama a fun��o que gera os v�rtices da esfera, e guarda em Vertices
	GenerateSphereMesh(100, Vertices, Triangles);
	// obter a qtde de v�rtices gerados
	numVertices = Vertices.size();
	// obter a qtde do vetor de �ndices
	numIndices = Triangles.size() * 3; // multiplica por 3 pois s�o 3 coordenadas

	GLuint VertexBuffer; // cria uma vari�vel que representa o Buffer da PV
	// ativei a vari�vel de buffer
	glGenBuffers(1, &VertexBuffer);
	// associa o buffer como sendo um buffer da placa de v�deo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// copia os Vertex, que est�o dentro de Vertices
	glBufferData(GL_ARRAY_BUFFER, 
		Vertices.size() * sizeof(Vertex), 
		Vertices.data(), GL_STATIC_DRAW);

	// esta vari�vel vai conter o vetor de �ndices
	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer); // ativa o buffer

	// indica que ElementBuffer vai conter o vetor de �ndices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer); 

	// copia o conte�do para o buffer da placa de v�deo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(GLuint),
		Triangles.data(), GL_STATIC_DRAW);

	GLuint VAO; // cria o VAO da esfera
	// ativei o VAO
	glGenVertexArrays(1, &VAO);
	// VAO representa o buffer da PV
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); // habilita as coordenadas de posi��o
	glEnableVertexAttribArray(1); // habilita as coordenadas normalizada
	glEnableVertexAttribArray(2); // habilita as coordenadas de cor
	glEnableVertexAttribArray(3); // habilita as coordenadas de textura

								  // relaciono conte�do da placa de v�deo com o VertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// relaciona conte�do da placa de v�deo com ElementBuffer - vetor de �ndices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	// indica o local no buffer contendo as posi��es dos v�rtices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	// indica o local no buffer contendo o vetor normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, Normal)));

	// indica o local no buffer contendo as posi��es das cores
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, Color)));

	// indica o local no buffer contendo as posi��es das coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*> (offsetof(Vertex, UV)));

	// desabilita o bind
	glBindVertexArray(0);

	return VAO;

}



// criando uma classe chamada SimpleCamera
class SimpleCamera
{
public: // vari�veis e os m�todos ser�o p�blicis


	void MoveForward(float Amount)
	{
		// localiza��o da c�mera depende da dire��o
		Location += Direction * Amount * Speed;
	}

	void MoveRight(float Amount)
	{
		// localiza��o da c�mera depende do vetor right, que por sua vez depende
		// da dire��o e da orienta��o
		glm::vec3 Right = glm::cross(Direction, Up);
		Location += Right * Amount;
	}

	glm::mat4 GetView()
	{
		return glm::lookAt(Location, Location + Direction, Up);
	}

	// obtem a matriz de proje��o e de vis�o
	glm::mat4 GetViewProjection()
	{
		//glm::mat4 View = glm::lookAt(Location, Location + Direction, Up);
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * GetView();
	}


	void Look(float Yaw, float Pitch)
	{
		Yaw *= Sensitivity;
		Pitch *= Sensitivity;
		// criando o vetor Right a partir de Direction e do Up
		const glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
		// criando a matriz identidade
		const glm::mat4 I = glm::identity<glm::mat4>();

		// criar a rota��o Yaw - em y , por isso o Up est� envolvido
		glm::mat4 YawRotation = glm::rotate(I, glm::radians(Yaw), Up);
		// criar a rota��o Pitch - em x, por isso o Right est� envolvido
		glm::mat4 PitchRotation = glm::rotate(I, glm::radians(Pitch), Right);

		// altera ou atualiza o vetor Up
		Up = PitchRotation * glm::vec4{ Up, 0.0f };
		// altera ou atualiza o vetor Direction 
		Direction = YawRotation * PitchRotation * glm::vec4{ Direction, 0.0f };
	}

	// localiza��o da c�mera
	glm::vec3 Location = { 0.0f, 0.0f, 5.0f };
	// dire��o da c�mera
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	// orienta��o da c�mera
	glm::vec3 Up = { 0.0f, 1.0f, 0.0f };

	// velocidade de movimenta��o
	float Speed = 5.0f;
	float Sensitivity = 0.1f;

	// campo de vis�o
	float FieldOfView = glm::radians(45.0f);
	// raz�o de aspecto
	float AspectRatio = Width / Height;
	// posi��o do campo mais perto
	float Near = 0.01f;
	// posi��o do campo mais longe
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

// cria um objeto da classe SimpleCamera
SimpleCamera Camera;
bool bEnableMouseMovement = false;
glm::vec2 PreviousCursor{0.0, 0.0};

// fun��o executada automaticamente quando o bot�o do mouse for clicado
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
			// obter as posi��o X e Y do mouse
			glfwGetCursorPos(Window, &X, &Y);
			// atualizo a posi��o do cursor anterior
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

// fun��o executada automaticamente quando o mouse for movimentado
void MouseMotionCallback(GLFWwindow* Window, double X, double Y)
{
	if (bEnableMouseMovement)
	{	
		// posi��o corrente do cursos
		glm::vec2 CurrentCursor{ X, Y };
		// posi��o anterior - posi��o atual
		glm::vec2 Delta = PreviousCursor - CurrentCursor;

		 std::cout << glm::to_string(Delta) << std::endl;
		// atualiza a c�mera - matriz de vis�o
		Camera.Look(-Delta.x, -Delta.y);
		// posi��o anterior ser� a posi��o corrente
		PreviousCursor = CurrentCursor;
	}
}

void ResizeCallback(GLFWwindow* Window, int Width, int Height)
{
	// vai redefinir a viewport
	Camera.AspectRatio = static_cast<float>(Width) / Height;
	glViewport(0, 0, Width, Height);
}



int main() {

	glfwInit();

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	
	// altera a propor��o da elipse de acordo com resize da janela
	glfwSetFramebufferSizeCallback(Window, ResizeCallback);

	// registra a fun��o de callback MouseButtonCallback
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, MouseMotionCallback);


	glfwMakeContextCurrent(Window);

	// permite que os fps sejam alterados, para mais lento ou mais r�pido
	// n�o ultrapassa o limite da placa de v�deo
	glfwSwapInterval(0);

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


	ResizeCallback(Window, Width, Height);

	// chama a fun��o ReadFile para ler o conte�do dos arquivos shaders
	std::string VertexShaderSource = ReadFile("shaders/triangle_vert.glsl");
	std::cout << VertexShaderSource << std::endl;
	std::string FragmentShaderSource = ReadFile("shaders/triangle_frag.glsl");
	std::cout << FragmentShaderSource << std::endl;

	// cria um programa composto de vertex shader e do fragment shader
	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	// Carregar a Textura para a Mem�ria de V�deo
	GLuint TextureId = LoadTexture("textures/earth_2k.jpg");

	GLuint QuadVAO = LoadGeometry();

	GLuint SphereNumVertices = 0; // qtde de vetores da elipse

	GLuint SphereNumIndices = 0; // qtde de �ndices do vetor de tri�ngulos da elipse

	GLuint SphereVAO = LoadSphere(SphereNumVertices, SphereNumIndices);

	std::cout << "N�mero de v�rtices da esfera " << SphereNumVertices << std::endl;
	std::cout << "N�mero de �ndices da esfera " << SphereNumIndices << std::endl;

	// Model View Projection
	//glm::mat4 ModelMatrix = glm::identity<glm::mat4>();
	
	glm::mat4 Model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(180.0f), glm::vec3(1, 0, 0));

	// criando uma ModelMatrix para a segunda esfera
	// glm::mat4 ModelMatrix2 = glm::translate(I, glm::vec3(1, 0, 0));



	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// habilita teste de profundiade - z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // desenha o que est� mais perto da c�mera
	// Define a cor de fundo
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);

	// � em termos de fps
	// � o tempo que houve a �ltima atualiza��o de frame
	double PreviousTime = glfwGetTime();

	// cria uma fonte de luz direcional
	// Criar uma fonte de luz direcional
	DirectionalLight Light;
	Light.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	Light.Intensity = 1.0f;

	while (!glfwWindowShouldClose(Window)) {
		// � o tempo que ocorreu a atualiza��o de frame atual
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
// 
 		//   glClear(GL_COLOR_BUFFER_BIT);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		 // ativar o programa do shader
		glUseProgram(ProgramId);

		// criando a matriz normal
		glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection() * Model;

		glm::mat4 ModelViewProjection = ViewProjectionMatrix * Model;
		// -ilumina��o
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(Camera.GetView() * Model));

		// processamento sendo executado na GPU
		// obtemos a localiza��o do uniforme ModelViewProjection
		GLint ModelViewProjectionLoc = 
			glGetUniformLocation(ProgramId, "ModelViewProjection");

		// ilumina��o
		GLint NormalMatrixLoc =
			glGetUniformLocation(ProgramId, "NormalMatrix");

		// insere o uniforme na GPU
		/// primeira esfera
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, 
			glm::value_ptr(ModelViewProjection));

		glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE,
			glm::value_ptr(NormalMatrix));

		// ativa a TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);
	
		// envia  os dados da luz direcional para o vertex shader - dire��o
		GLint LightDirectionLoc = glGetUniformLocation(ProgramId, "LightDirection");
		glUniform3fv(LightDirectionLoc, 1, 
			glm::value_ptr(Camera.GetView() * glm::vec4(Light.Direction, 0.0f)));

		// envia  os dados da luz direcional para o vertex shader - intensidade
		GLint LightIntensityLoc = glGetUniformLocation(ProgramId, "LightIntensity");
		glUniform1f(LightIntensityLoc, Light.Intensity);

		// encapsular ou esconder a complexidade
		// glBindVertexArray(QuadVAO); 
		glBindVertexArray(SphereVAO);

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
		  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		 //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		 //glDrawArrays(GL_TRIANGLES, 0, Quad.size());
		 //glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, nullptr);
		  //glDrawArrays(GL_POINTS, 0, SphereNumVertices);
		   // primeira esfera
		glDrawElements(GL_TRIANGLES, SphereNumIndices , GL_UNSIGNED_INT, nullptr);

		// segunda esfera
		//glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE,
		//	glm::value_ptr(ModelViewProjection * ModelMatrix2));
		// segunda esfera
		// glDrawElements(GL_TRIANGLES, SphereNumIndices, GL_UNSIGNED_INT, nullptr);

		 glBindVertexArray(0); // desabilita o estado
		 // reverte o estado criado
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);

		glUseProgram(0);
		// processo todos os eventos da fila de eventos do GLFW
		// eventos: teclado, mouse, gamepad
		glfwPollEvents();
		// envia o conte�do do framebuffer da janela para ser desenhado na tela
		glfwSwapBuffers(Window);

		// tratamento de evento com teclado
		// tecla digitada foi o W -> ir pra frente
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Camera.MoveForward(1.0f * DeltaTime);
		}
		// move-se para tr�s
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
	//glDeleteBuffers(1, &VertexBuffer);

	glfwTerminate();

	return 0;
}

