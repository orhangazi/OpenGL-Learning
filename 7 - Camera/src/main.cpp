#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include "shader.hpp"
#include "stbiImageLoader.cpp"

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
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

// indexler
unsigned int indices[] = { // Note that we start from 0!
	0, 1, 3, // First triangle
	1, 2, 3 // Second triangle
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

	int windowWidth = 800;
	int windowHeight = 600;
	glViewport(0, 0, windowWidth, windowHeight);

	// Ekrandaki görüntünün çizim modunu değiştirebilen fonksiyon ekledim. Sadece çizgi şeklinde ve normal şekilde 2 ayurı mod seçilebiliyor. GL_LINE, GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::ifstream vFile("shaders/vertexshader.glsl");
	if (!vFile.is_open())
	{
		std::cout << "ERROR: vertexshader.glsl açılamadı" << std::endl;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Vertex Attributes texture coordinatları (2 birimlik: x, y)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
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

	// Transformations: 1 birim hareket ettirmek için trans = glm::mat4(1.0f) 
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;

	// çıktı ilk oluşturulan vektöre kaydırmak istediğimiz 1 birimlik vektörü çarpar ve çıktısını yine ilk oluşturduğumuz
	// vec vektörüne atar. Sonuçta (1+1, 0+1, 0+0) 210 yazmalıdır:
	std::cout << vec.x << vec.y << vec.z << std::endl;

	// Başka tranformation işlemleri:
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = glm::rotate(trans2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	trans2 = glm::scale(trans2, glm::vec3(0.5f, 0.5f, 0.5f));
	
	// elde ettiğimiz yeni pozisyon verileri vertex shader'a gönderiliyor.
	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, -0.5f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)};

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering commands here
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing code in render loop

		// Kamera oluşturuluyor ve ayarlanıyor.
		// Kamera pozisyonu ayarlanıyor (x, y, z):
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

		// Kamera yönünü ayarlanıyor (x, y, z):
		// The name direction vector is not the best chosen name, 
		// since it is actually pointing in the reverse direction of what it is targeting.
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

		// Sağ yönün ayarlanması:
		// Right axis
		// The next vector that we need is a right vector that represents 
		// the positive x-axis of the camera space. To get the right vector
		// we use a little trick by first specifying an up vector that points 
		// upwards (in world space). Then we do a cross product on the up vector 
		// and the direction vector from step 2. Since the result of a cross product 
		// is a vector perpendicular to both vectors, we will get a vector that points 
		// in the positive x-axis's direction (if we would switch the cross product 
		// order we'd get a vector that points in the negative x-axis):
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraLeft = glm::normalize(glm::cross(cameraDirection, up));

		// Yukarı yönün ayarlanması:
		// Up axis:
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		
		//tahmini cameraDown hesaplamam:
		glm::vec3 cameraDown = glm::cross(cameraRight, cameraDirection);

		// GLM'in LookAt matrisini kullanıyoruz. Her şeyi bizim için çok daha kolaylaştırıy.
		// Sadece camera pozisyonu, kamera hedefi ve yukarı matris (up):
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		//view = glm::lookAt(cameraPos, cameraTarget, up);
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		// Uniform ourColor'ı güncelleyerek fragment shader'ı güncelliyoruz.
		// Yeşilin tonlarını her frame yeniden hesaplayıp geçişli renk ayarlıyoruz.
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Uniform ile shaderProgram sürecinde rengi değiştirilebiliyor. Veriler güncellenebiliyor yani.
		// 3D'de ilk model matrisimizin oluşturulması: model yere değecekmiş gibi aşağı indiriliyor.
		// glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Sahne içinde nasıl hareket edildiğini sonra göstereceğiz daha ayrıntılı olarak.
		// Şimdilik view matrix şöyle görünüyor:
		//glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		// sol olarak perpective projection ayarlıyoruz. En gerçekçi görünüm için 45.0f:
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f))

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); // zamana bağlı dönme

		// matrisler vertex shader'a gönderiliyor:
		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// view matrisleri vertex shadera gönderiliyor:
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// Projection matrisleri vertex shadera gönderiliyor:
		int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Başka başka tranformation işlemleri:
		glm::mat4 trans3 = glm::mat4(1.0f);
		//trans3 = glm::scale(trans3, glm::vec3(0.5f, 0.5f, 0.5f));
		trans3 = glm::translate(trans3, glm::vec3(0.5f, -0.5f, 0.0f));
		trans3 = glm::rotate(trans3, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// elde ettiğimiz yeni pozisyon verileri vertex shader'a gönderiliyor.
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans3));

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
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}