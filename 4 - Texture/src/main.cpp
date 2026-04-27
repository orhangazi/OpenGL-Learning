#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.chh"
#include "stbiImageLoader.chh"

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
	// positions          // colors           // texture coords
	0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = { // Note that we start from 0!
	0, 1, 3, // First triangle
	1, 2, 3 // Second triangle
};

float texCoords[] = {
	0.0f, 0.0f, // lower-left corner
	1.0f, 0.0f, // lower-right corner
	0.5f, 0.0f // top-center corner
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
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Element buffer bağlanır ve bu buffer'a indices (indexler) kopyalanır.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader ourShader("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	/* float borderColor[] = {
		1.0f, 1.0f, 0.0f, 1.0f
	};

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */

	// 1. then set the vertex attributes pointers
	// Vertex Attributes köşe bilgisini (vertex: 3 birimlik köşe) göndermek için bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	// 2. then set the vertex attributes pointers
	// Vertex Attributes renk bilgisi (3 birimlik: rgb) bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// 3. then set the vertex attributes pointers
	// Vertex Attributes texture coordinatları (2 birimlik: x, y) bilgisini göndermek için bağlanıyor ve etkinleştiriliyor.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Texture oluşturuluyor
	unsigned int texture1;
	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on the currently bound texture1 object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture1
	int width1, height1, nrChannels1;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data1 = stbi_load("textures/container.jpg", &width1, &height1, &nrChannels1, 0);

	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}

	// load and generate the second texture
	// Texture oluşturuluyor
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	int width2, height2, nrChannels2;
	unsigned char *data2 = stbi_load("textures/awesomeface.png", &width2, &height2, &nrChannels2, 0);

	if(data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}

	stbi_image_free(data1);
	stbi_image_free(data2);

	// Draw the object:
	ourShader.use(); // don't forget to activate the shader the shader before settings uniforms!
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); //set it manually
	ourShader.setInt("texture2", 1); //or with shader class

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

		// Uniform ile shaderProgram sürecinde rengi değiştirilebiliyor. Veriler güncellenebiliyor yani.
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Texture ayarlanıyor:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Bir yukarıda VAO tanımlandığında, bir de şimdi burda render döngüsü içinde glUseProgram
		// fonksiyonundan sonra bind ediliyor. Sebebini şimdilik anlamadım.
		//glBindVertexArray(VAO);
		glBindVertexArray(VAO);

		// Üçgen çizdirildiği için 3 tane vertex çizileceğini söylüyoruz. 2 tane üçgen çizilseydi
		// 2. parametre olarak 6 tane vertex çizilmesini belirtirdik:
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}