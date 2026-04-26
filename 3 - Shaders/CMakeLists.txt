cmake_minimum_required(VERSION 3.10...3.29)
project(Shaders)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL root path'leri
set(GLFW3_PATH "C:/glfw-3.4")
#set(SDL2_PATH "C:/SDL2-2.30.11")
#set(SDL2_IMAGE_PATH "C:/SDL2_image-2.6.0")
#set(SDL2_MIXER_PATH "C:/SDL2_mixer-2.8.1")
#set(SDL2_TTF_PATH "C:/SDL2_ttf-2.24.0")
#set(GLEW_PATH "C:/glew-2.3.1")
#set(GLM_PATH "C:/glm-0.9.9.8")

find_package(OpenGL REQUIRED)

add_executable(Shaders
    src/main.cpp
    src/shader.cpp
    src/glad.c
    #src/GameState.cpp
    #src/Player.cpp
    #src/Enemy.cpp
    #src/Bullet.cpp
    #src/Particle.cpp
    #src/Level.cpp
    #src/ResourceManager.cpp
    #src/Shader.cpp
    #src/Model.cpp
    #src/Camera.cpp
)

# Include path'ler
target_include_directories(Shaders PRIVATE
    ${GLFW3_PATH}/include
    #${SDL2_PATH}/include
    #${SDL2_IMAGE_PATH}/include
    #${SDL2_MIXER_PATH}/include
    #${SDL2_TTF_PATH}/include
    #${GLEW_PATH}/include
    #${GLM_PATH}
    include
)

# Lib path'ler
link_directories(
    #${SDL2_PATH}/lib/x64
    #${SDL2_IMAGE_PATH}/lib/x64
    #${SDL2_MIXER_PATH}/lib/x64
    #${SDL2_TTF_PATH}/lib/x64
    #${GLEW_PATH}/lib/Release/x64
    ${GLFW3_PATH}/build/src/Debug
)

# 🔥 Asıl kritik kısım
target_link_libraries(Shaders
    #${SDL2_PATH}/lib/x64/SDL2.lib
    #${SDL2_PATH}/lib/x64/SDL2main.lib
    #${SDL2_IMAGE_PATH}/lib/x64/SDL2_image.lib
    #${SDL2_MIXER_PATH}/lib/x64/SDL2_mixer.lib
    #${SDL2_TTF_PATH}/lib/x64/SDL2_ttf.lib
    opengl32
    #${GLEW_PATH}/lib/Release/x64/glew32.lib
    ${GLFW3_PATH}/build/src/Debug/glfw3.lib
)

install(TARGETS Shaders DESTINATION bin)

# Shader dosyalarını build klasörüne kopyala
add_custom_command(TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:Shaders>/shaders
)