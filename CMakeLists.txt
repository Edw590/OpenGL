cmake_minimum_required(VERSION 3.31)
project(OpenGL C)

set(CMAKE_C_STANDARD 23)

add_compile_definitions(GLEW_STATIC)

set(LIBS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/include")
# Add include directories
include_directories("${LIBS_INCLUDE_DIR}")

# Add the executable
add_executable(OpenGL
        main.c
        glad.c
        Utils/UtilsStrings.c
        Utils/UtilsStrings.h
        stb.c
        Engine/Shader.c
        Engine/Shader.h
        Utils/UtilsOpenGL.c
        Utils/UtilsOpenGL.h)

# Link libraries
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/Libraries/libs")
set(MINGW_LIBS_DIR "C:/msys64/mingw64/lib")
target_link_libraries(OpenGL
        "${MINGW_LIBS_DIR}/libglew32.dll.a"
        "${LIBS_DIR}/libglfw3.a"
        opengl32
        gdi32
        user32
        shell32
)
