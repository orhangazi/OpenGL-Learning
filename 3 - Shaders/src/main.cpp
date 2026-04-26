#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.chh"

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
/* float vertices[] = {
	// first triangle // color
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f // top left
}; */

float vertices[] = {
	// first triangle // color
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top
};

unsigned int indices[] = { // Note that we start from 0!
	0, 1, 3, // First triangle
	1, 2, 3, // Second triangle
};

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

	// Ekrandaki görüntünün çizim modunu değiştirebilen fonksiyon ekledim. Sadece çizgi şeklinde ve normal şekilde 2 ayurı mod seçilebiliyor. GL_LINE, GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::ifstream vFile("shaders/vertexshader.glsl");
	if (!vFile.is_open())
	{
		std::cout << "ERROR: vertexshader.glsl acilamadi" << std::endl;
		return -1;
	}
	std::stringstream vss;
	vss << vFile.rdbuf();
	vFile.close();
	std::string vertexShaderStr = vss.str();
	const char *vertexShaderSource = vertexShaderStr.c_str();

	std::ifstream fFile("shaders/fragmentshader.glsl");
	
	if (!fFile.is_open())
	{
		std::cout << "ERROR: fragmentshader.glsl acilamadi" << std::endl;
		return -1;
	}
	
	std::stringstream fss;
	fss << fFile.rdbuf();
	fFile.close();

	std::string fragmentShaderStr = fss.str();
	const char *fragmentShaderSource = fragmentShaderStr.c_str();

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
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	// Element buffer bağlanır ve bu buffer'a indices (indexler) kopyalanır.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader ourShader("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");

	// 1. then set the vertex attributes pointers
	// Vertex Attributes bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	// 1. then set the vertex attributes pointers
	// Vertex Attributes bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering commands here
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing code in render loop

		// Uniform ourColor'ı güncelleyerek fragment shader'ı güncelliyoruz.
		// Yeşilin tonlarını her frame yeniden hesaplayıp geçişli renk ayarlıyoruz.
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Draw the object:
		ourShader.use();
		ourShader.setFloat("someUniform", 1.0f);

		// Uniform ile shaderProgram sürecinde rengi değiştirilebiliyor. Veriler güncellenebiliyor yani.
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Bir yukarıda VAO tanımlandığında, bir de şimdi burda render döngüsü içinde glUseProgram
		// fonksiyonundan sonra bind ediliyor. Sebebini şimdilik anlamadım.
		glBindVertexArray(VAO);

		// Üçgen çizdirildiği için 3 tane vertex çizileceğini söylüyoruz. 2 tane üçgen çizilseydi
		// 2. parametre olarak 6 tane vertex çizilmesini belirtirdik:
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}