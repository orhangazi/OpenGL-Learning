#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <camera.h>
#include <model.hpp>

#include <iostream>
#include <filesystem.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (befor loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	// Depth test: fragment'ların z değerlerine göre önde/arkada olup olmadığını kontrol eder.
	glEnable(GL_DEPTH_TEST);
	// Stencil test: her fragment için stencil buffer'daki değeri kontrol eder,
	// bu değere göre fragment'ı çizer ya da atar. Border efekti için kullanacağız.
	glEnable(GL_STENCIL_TEST);
	// glStencilOp(stencil test başarısız, depth test başarısız, ikisi de başarılı):
	// GL_KEEP  → mevcut stencil değerini koru
	// GL_REPLACE → stencil değerini glStencilFunc'ta verilen ref değeriyle değiştir
	// Yani: stencil ve depth test geçerse buffer'a ref değerini yaz, diğer durumlarda dokunma.
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Shader shaderSingleColor("shaders/shaderSingleColor.vs", "shaders/shaderSingleColor.fs");

	// build and compile our shader zprogram
	// ------------------------------------
	// Shader lightingShader("shaders/1.colors.vs", "shaders/1.colors.fs");
	// Shader lightCubeShader("shaders/1.light_cube.vs", "shaders/1.light_cube.fs");
	Shader ourShader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");

	// load models
	// -----------
	Model ourModel(FileSystem::getPath("resources/objects/backpack/backpack.obj"));

	// draw in wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		// Her frame başında renk, derinlik ve stencil buffer'larını temizle.
		// Stencil buffer temizlenmezse önceki frame'den kalan 1'ler border'ı bozar.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setVec3("viewPos", camera.Position);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));

		// --- 1. GEÇİŞ: Asıl modeli çiz, stencil buffer'a 1 yaz ---
		// GL_ALWAYS: stencil testi her zaman geçer, yani tüm fragment'lar çizilir.
		// 1: stencil buffer'a yazılacak referans değer.
		// 0xFF: mask — tüm bitleri yaz (0xFF = 11111111, hiçbir biti engelleme).
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// 0xFF mask ile stencil buffer'a yazmaya izin ver.
		// glStencilOp'ta GL_REPLACE demiştik, yani "modelin çizildiği" her piksele 1 yazılır.
		glStencilMask(0xFF);
		ourShader.use();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// --- 2. GEÇİŞ: Büyütülmüş "modeli" tek renkle çiz, border oluştur ---
		// GL_NOTEQUAL: stencil buffer değeri 1'e EŞİT OLMAYAN pikselleri çiz.
		// Yani modelin tam üstündeki pikseller (stencil=1) atlanır,
		// sadece taşan kısım (stencil=0) çizilir → bu taşan kısım border olur.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// 0x00 mask ile stencil buffer'a yazmayı kapat.
		// Border çizerken stencil buffer'ı değiştirmek istemiyoruz.
		glStencilMask(0x00);
		// Depth test'i kapat: border modeli arkada kalsa bile görünsün.
		// Açık bırakırsak border başka nesnelerin arkasında kaybolabilir.
		glDisable(GL_DEPTH_TEST);

		glm::mat4 modelBorder = glm::mat4(1.0f);
		modelBorder = glm::translate(modelBorder, glm::vec3(0.0f, 0.0f, 0.0f));
		modelBorder = glm::scale(modelBorder, glm::vec3(0.75f, 0.75f, 0.75f)); // asıl model 0.7f, border 0.75f → fark border kalınlığı

		shaderSingleColor.use();
		shaderSingleColor.setMat4("projection", projection);
		shaderSingleColor.setMat4("view", view);
		shaderSingleColor.setMat4("model", modelBorder);
		ourModel.Draw(shaderSingleColor);

		// Border çizimi bitti, bir sonraki frame için her şeyi sıfırla:
		// Stencil mask'i tekrar aç (glClear stencil buffer'ı temizleyebilsin).
		glStencilMask(0xFF);
		// Stencil testi GL_ALWAYS yap ki bir sonraki frame'de her şey normal çizilsin.
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		// Depth test'i geri aç.
		glEnable(GL_DEPTH_TEST);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
