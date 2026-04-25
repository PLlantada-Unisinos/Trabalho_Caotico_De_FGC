/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para as disciplinas de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 07/03/2025
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

//Valores p/rotação
bool rotateX=false, rotateY=false, rotateZ=false;
bool reverseX=false, reverseY=false, reverseZ=false;

//Valores p/camera
glm::vec3 CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;

void createIcosahedron(std::vector<GLfloat>& data,
                       std::vector<GLuint>& indices)
{

	const GLfloat phi = (1.0F + sqrt(5.0f)/2.0f);
	std::vector<std::vector<GLfloat>> vertices = {
		//x    y    z   
		 {-1,  phi,  0 }, 
		 { 1,  phi,  0 }, 
		 {-1, -phi,  0 }, 
		 { 1, -phi,  0 },

		 {0,  -1,  phi},
		 {0,   1,  phi},
		 {0,  -1, -phi},
		 {0,   1, -phi},

		 { phi, 0,  -1},
		 { phi, 0 ,  1},
		 {-phi, 0 , -1},
		 {-phi, 0 ,  1}
	};

	 for (size_t i = 0; i < vertices.size(); i++) {
        float len = std::sqrt(vertices[i][0]*vertices[i][0] + vertices[i][1]*vertices[i][1] + vertices[i][2]*vertices[i][2]);
        float x = vertices[i][0] / len;
        float y = vertices[i][1] / len;
        float z = vertices[i][2] / len;

        // posição
        data.push_back(x);
        data.push_back(y);
        data.push_back(z);

        // cor
        data.push_back((x + 1.0f) / 2.0f);
        data.push_back((y + 1.0f) / 2.0f);
        data.push_back((z + 1.0f) / 2.0f);
    }

	//vertices-teste antigo
	GLfloat vertices2[] = {

		  //x    y    z    r    g    b
		  0.5, 0.5, 0.5,  1.0, 1.0, 1.0,
		  0.5, 0.5, -0.5, 1.0, 1.0, 1.0,
		 -0.5, 0.5, 0.5,  1.0, 1.0, 1.0,

		  0.5, 0.5, -0.5, 1.0, 1.0, 1.0,
		 -0.5, 0.5, -0.5, 1.0, 1.0, 1.0,
		 -0.5, 0.5,  0.5, 1.0, 1.0, 1.0,

		 //
		  0.5, 0.5, 0.5, 1.0, 1.0, 1.0,
		  0.0,-0.5, 0.0, 1.0, 1.0, 1.0,
		 -0.5, 0.5, 0.5, 1.0, 1.0, 1.0,

		  0.5, 0.5, 0.5, 1.0, 0.0, 1.0,
		  0.0,-0.5, 0.0, 1.0, 0.0, 1.0,
		  0.5, 0.5,-0.5, 1.0, 0.0, 1.0,

		  0.5, 0.5, -0.5, 1.0, 1.0, 1.0,
		  0.0,-0.5,  0.0, 1.0, 1.0, 1.0,
		 -0.5, 0.5, -0.5, 1.0, 1.0, 1.0,

		 -0.5, 0.5, -0.5, 1.0, 1.0, 1.0,
		  0.0, -0.5, 0.0, 1.0, 1.0, 1.0,
		 -0.5, 0.5, 0.5,  1.0, 1.0, 1.0,


	};

	indices = {
	0,11,5,  0,5,1,  0,1,7,  0,7,10, 0,10,11,
    1,5,9,   5,11,4, 11,10,2,10,7,6, 7,1,8,
    3,9,4,   3,4,2,  3,2,6,  3,6,8,  3,8,9,
    4,9,5,   2,4,11, 6,2,10, 8,6,7,  9,8,1
	};

	/*GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


	//Atributo posição (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);*/
}

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalhin", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION); 
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo

	glUseProgram(shaderID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp); //matriz camera

	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	
	model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	model = glm::mat4(1);

	glEnable(GL_DEPTH_TEST);

	std::vector<GLfloat> data;
	std::vector<GLuint> indices;

	createIcosahedron(data, indices);
 	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	//Atributo posição (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = 0.05f;

		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			
		}
		if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}
		if (reverseX)
		{

			model = glm::rotate(model, angle, glm::vec3(-1.0f, 0.0f, 0.0f));

		}
		if (reverseY)
		{

			model = glm::rotate(model, angle, glm::vec3(0.0f, -1.0f, 0.0f));

		}
		if (reverseZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, -1.0f));

		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_LINES, 0, 20);
		
		glDrawArrays(GL_POINTS, 0, 12);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W)
	{
		switch (action) {

			case GLFW_PRESS:
				rotateX = true; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

		case GLFW_REPEAT:
				rotateX = true; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}

	if (key == GLFW_KEY_D)
	{
		switch (action) {

			case GLFW_PRESS:
				rotateX = false; 	reverseX = false;
				rotateY = true;		reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

		case GLFW_REPEAT:
				rotateX = false; 	reverseX = false;
				rotateY = true;	    reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}

	if (key == GLFW_KEY_R)
	{
		switch (action) {

			case GLFW_PRESS:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = true;	    reverseZ = false;
		break;

		case GLFW_REPEAT:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = true;	    reverseZ = false;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}

	if (key == GLFW_KEY_S)
		{
		switch (action) {

			case GLFW_PRESS:
				rotateX = false; 	reverseX = true;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

		case GLFW_REPEAT:
				rotateX = false; 	reverseX = true;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}
		
	if (key == GLFW_KEY_A)
		{
		switch (action) {

			case GLFW_PRESS:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = true;
				rotateZ = false;	reverseZ = false;
		break;

		case GLFW_REPEAT:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = true;
				rotateZ = false;	reverseZ = false;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}

	if (key == GLFW_KEY_T)
		{
		switch (action) {

			case GLFW_PRESS:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = true;
		break;

		case GLFW_REPEAT:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = true;
		break;

			case GLFW_RELEASE:
				rotateX = false; 	reverseX = false;
				rotateY = false;	reverseY = false;
				rotateZ = false;	reverseZ = false;
		break;
		}
	}


}

int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}