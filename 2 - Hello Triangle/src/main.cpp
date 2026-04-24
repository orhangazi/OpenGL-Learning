#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// çerçeve boyutu değiştirildiğinde viewport dimensionlarını ayarlar
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Kullanıcı tarafından yapılan inputları işler
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Üçgeni oluşturan köşe bilgileri (Vertex data)
float vertices[] = {
	// first triangle
	0.5f, 0.5f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f // top left
};

unsigned int indices[] = { // Note that we start from 0!
	0, 1, 3, // First triangle
	1, 2, 3, // Second triangle
};

const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"void main()\n"
									"{\n"
									"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need. Note that on Mac OS X you need to add glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); to your initialization code for it to work:
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 1. Vertex Array Object oluşturuluyor
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. Vertex Buffer Object oluşturuluyor
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// glBindBuffer ve glBufferData kullanarak:
	// 0. copy our vertices array in a buffer for OpenGL to use
	// VBO'yu GL_ARRAY_BUFFER hedefine bağlıyoruz:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Buffer bağlandıktan sonra, üçgeni oluşturan köşeler olan (yukarıda tanımladık)
	// vertex data'yı buffer hafızasına kopyalıyoruz.
	// GPU vertex data'ya anında erişebilir olacak ve performans çok artmış olacak.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. Element Buffer Object oluşturuluyor.
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Element buffer bağlanır ve bu buffer'a indices (indexler) kopyalanır.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex shader yükleniyor
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int successVertex;
	char infoLogVertex[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);

	if (!successVertex)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLogVertex << std::endl;
	}

	// Fragment shader yükleniyor
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int successFragment;
	char infoLogFragment[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragment);

	if (!successFragment)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragment << std::endl;
	}

	// Shader Program oluşturuluyor ve yukarıdaki derlenmiş vertex ve fragment shaderlar birbiriyle bağlanacak.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int successShaderProgram;
	char infoLogShaderProgram[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successShaderProgram);

	if (!successShaderProgram)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogShaderProgram);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLogShaderProgram << std::endl;
	}

	// Performans için shaderlar linklendikten sonra onları siliyoruz:
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 1. then set the vertex attributes pointers
	// Vertex Attributes bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering commands here
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing code in render loop

		// Draw the object:
		// Bir objeyi render ederken (ekranda işlerken) shaderProgram kullanılır (use).
		// 2. use our shader program when we want to render an object
		glUseProgram(shaderProgram);

		// Bir yukarıda VAO tanımlandığında, bir de şimdi burda render döngüsü içinde glUseProgram
		// fonksiyonundan sonra bind ediliyor. Sebebini şimdilik anlamadım.
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}